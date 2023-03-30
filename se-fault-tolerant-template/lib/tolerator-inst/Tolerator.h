
#pragma once


#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"


namespace tolerator {


enum class AnalysisType {
  LOGGING,
  IGNORING,
  DEFAULTING,
  BYPASSING
};


struct Tolerator : public llvm::ModulePass {
  static char ID;
  AnalysisType analysisType;

  Tolerator(AnalysisType analysisType) : llvm::ModulePass(ID), analysisType(analysisType) {}

  bool runOnModule(llvm::Module& m) override;

  void handleDivision(std::vector<llvm::Instruction*> &divisionList, llvm::FunctionCallee customFunction, int analysisType);
  void handleMemoryAllocation(std::vector<llvm::Instruction*> &allocationList, llvm::FunctionCallee customFunction, int analysisType);
};


}  // namespace tolerator


