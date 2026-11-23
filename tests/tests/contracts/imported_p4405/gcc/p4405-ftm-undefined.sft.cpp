//remark: imported from gcc:p4405-ftm-undefined.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// P4405: without -fcontracts-p4405 the feature-test macro is not defined
// (the umbrella -fcontracts-p3850 enables it, so use a bare -fcontracts).
//
// Mirror: clang/test/Contracts/p4405-ftm-undefined.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

#ifdef __cpp_contracts_prove
#error "__cpp_contracts_prove defined without -fcontracts-p4405"
#endif

int f (int x) pre (x > 0) { return x; }
