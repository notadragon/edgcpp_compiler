//remark: imported from gcc:p4298-ftm-undefined.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// negative gate -- without -fcontracts-p4298 the nonthrowing-semantics
// feature-test macro is not defined (the umbrella -fcontracts-p3850 enables it,
// so use a bare -fcontracts here).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

#ifdef __cpp_contracts_nonthrowing_semantics
#error "__cpp_contracts_nonthrowing_semantics defined without -fcontracts-p4298"
#endif

int f (int x) pre (x > 0) { return x; }
int main () { return f (1); }
