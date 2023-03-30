

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "Tolerator.h"
#include <iostream>

using namespace llvm;
using tolerator::Tolerator;


namespace tolerator {

char Tolerator::ID = 0;

}


bool
Tolerator::runOnModule(Module& m) {
  auto& context = m.getContext();

  // This analysis just prints a message when the program starts or exits.
  // You should modify this code as you see fit.
  int analysisNum;
 
  switch(Tolerator::analysisType) {
    case AnalysisType::LOGGING:
      analysisNum = 1;
      break;
    case AnalysisType::IGNORING:
      analysisNum = 2;
      break;
    case AnalysisType::DEFAULTING:
      analysisNum = 3;
      break;
    case AnalysisType::BYPASSING:
      analysisNum = 4;
      break;
    default:
      analysisNum = 0;
  }
  
  auto* voidTy = Type::getVoidTy(context);
  auto* boolTy = Type::getInt1Ty(context);
  auto* int8Ty = Type::getInt8Ty(context);
  auto* int32Ty = Type::getInt32Ty(context);
  auto* sizeTy = Type::getInt64Ty(context);
  Type* voidPtrTy = voidTy->getPointerTo();

  auto* divTy = FunctionType::get(int32Ty, {int32Ty, int32Ty, int32Ty}, false);
  auto divisionWithZero = m.getOrInsertFunction("ToLeRaToR_divisionWithZero", divTy);

  auto* safeMallocTy = FunctionType::get(voidPtrTy, {sizeTy, int32Ty}, false);
  auto safeMalloc = m.getOrInsertFunction("ToLeRaToR_safeMalloc", safeMallocTy);

  auto* safeFreeTy = FunctionType::get(voidTy, {voidPtrTy, int32Ty}, false);
  auto safeFree = m.getOrInsertFunction("ToLeRaToR_safeFree", safeFreeTy);

  std::vector<Instruction *> divisionToReplace;
  std::vector<Instruction *> mallocToReplace;
  std::vector<Instruction *> freeToReplace;

  for (auto& f : m) {
    if (f.isDeclaration()) {
      continue;
    }
    for (auto& bb : f) {
      for (auto& i : bb) {

        CallBase *cb = dyn_cast<CallBase>(&i);
        if (!cb) {
          if (BinaryOperator* bin_op = dyn_cast<BinaryOperator>(&i)) {
            if (bin_op->getOpcode() == Instruction::SDiv || bin_op->getOpcode() == llvm::Instruction::UDiv) {
              divisionToReplace.push_back(&i);
            }
          }
        } 
        else {
          if (Function *called = cb->getCalledFunction()) {
            if (!called->getName().compare("malloc")) {
              mallocToReplace.push_back(&i);
            }
            if (!called->getName().compare("free")) {
              freeToReplace.push_back(&i);
            }
          }
        }
      }
    }
  }
  // Replace divides, 
  handleDivision(divisionToReplace, divisionWithZero, analysisNum);
  // Replace malloc
  handleMemoryAllocation(mallocToReplace, safeMalloc, analysisNum);
  // Replace free
  handleMemoryAllocation(freeToReplace, safeFree, analysisNum);

  return true;
}


void
Tolerator::handleDivision(std::vector<Instruction*> &divideList, FunctionCallee f, int analysisNum) {
  for (Instruction *i : divideList) {
    Value *numerator = i->getOperand(0);
    Value *divisor = i->getOperand(1);
    ConstantInt *analysisNumValue = ConstantInt::get(Type::getInt32Ty(i->getModule()->getContext()), analysisNum);
    
    CallInst *customDivide = CallInst::Create(f, {numerator, divisor, analysisNumValue});
    ReplaceInstWithInst(i, customDivide);
  }
}

void
Tolerator::handleMemoryAllocation(std::vector<Instruction*> &mallocList, FunctionCallee f, int analysisNum) {
  for (Instruction *i : mallocList) {
    ConstantInt *analysisNumValue = ConstantInt::get(Type::getInt32Ty(i->getModule()->getContext()), analysisNum);
    CallInst *safeMemoryCall = CallInst::Create(f, {i->getOperand(0), analysisNumValue});
    ReplaceInstWithInst(i, safeMemoryCall);
  }
}