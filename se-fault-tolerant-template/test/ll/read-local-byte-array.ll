; ModuleID = '<stdin>'
source_filename = "/workspaces/ex5/se-fault-tolerant-template/test/c/read-local-byte-array.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@__const.readByte.bytes = private unnamed_addr constant [5 x i8] c"\09\0B\0D\11\13", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i8 @readByte(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca [5 x i8], align 1
  store i32 %0, ptr %2, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %3, ptr align 1 @__const.readByte.bytes, i64 5, i1 false)
  %4 = load i32, ptr %2, align 4
  %5 = zext i32 %4 to i64
  %6 = getelementptr inbounds [5 x i8], ptr %3, i64 0, i64 %5
  %7 = load i8, ptr %6, align 1
  ret i8 %7
}

; Function Attrs: argmemonly nocallback nofree nounwind willreturn
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { argmemonly nocallback nofree nounwind willreturn }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 15.0.6"}
