//remark: imported from gcc:p4451-std-cxx26-no-p4451.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --no_contracts_p4451
//match_regex: ", line 22: (?:catastrophic )?error
// P4451 is enabled by C++26 unless -fno-contracts-p4451 is given: then
// the feature-test macros have P2900's values, and a specifier naming
// `enforced' is pointed at -fcontracts-p4451.
// { dg-do compile }
// { dg-options "-std=c++26 -fno-contracts-p4451" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

#if __cpp_contracts != 202502L || __cpp_lib_contracts != 202502L
#error "P2900's values"
#endif
#ifdef __gcc_contracts_p4451
#error "__gcc_contracts_p4451"
#endif

void f (int x) pre<std::contracts::controls::enforced> (x > 0);	// { dg-error "requires .-fcontracts-p4451." }
