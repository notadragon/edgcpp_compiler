//remark: imported from gcc:p4451-diag-no-flag.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --no_contracts_p4451
//match_regex: ", line 21: (?:catastrophic )?error
//match_regex: ", line 22: (?:catastrophic )?error
//match_regex: ", line 23: (?:catastrophic )?error
//match_regex: ", line 24: (?:catastrophic )?error
//match_regex: ", line 25: (?:catastrophic )?error
//match_regex: ", line 27: (?:catastrophic )?error
// P4451: without -fcontracts-p4451 (or -fcontracts-p3400, which implies it;
// C++26 enables it, so it is turned off here), an assertion-control-specifier
// is rejected, and one naming `enforced', unqualified or qualified, is pointed
// at the option.  Nor does <contracts> declare the controls namespace.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fno-contracts-p4451" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

void f (int x) pre<enforced> (x > 0);	// { dg-error "assertion-control-specifier naming .enforced. requires .-fcontracts-p4451." }
void g (int x) pre<other> (x > 0);	// { dg-error "assertion-control labels require .-fcontracts-p3400." }
void h (int x) { contract_assert<enforced> (x > 0); }	// { dg-error "requires .-fcontracts-p4451." }
void i (int x) pre<std::contracts::controls::enforced> (x > 0);	// { dg-error "assertion-control-specifier naming .enforced. requires .-fcontracts-p4451." }
void j (int x) pre<::std::contracts::controls::enforced> (x > 0);	// { dg-error "assertion-control-specifier naming .enforced. requires .-fcontracts-p4451." }

auto t = std::contracts::controls::enforced;	// { dg-error "controls" }

#if __cpp_contracts != 202502L || __cpp_lib_contracts != 202502L
#error "P2900's values"
#endif
#ifdef __gcc_contracts_p4451
#error "no P4451"
#endif
