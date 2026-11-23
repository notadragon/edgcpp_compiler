//remark: imported from gcc:result-name-attributes.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// A result-name-introducer is an attributed-identifier followed by `:', so
// the result name may carry an attribute-specifier-seq.
//
// Mirror: clang/test/Contracts/result-name-attributes.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

int f (int x) post (r [[maybe_unused]] : r > 0) { return x; }
