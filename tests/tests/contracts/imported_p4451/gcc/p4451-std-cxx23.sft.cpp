//remark: imported from gcc:p4451-std-cxx23.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++23 --contracts
//match_regex: ", line 22: (?:catastrophic )?error
// P4451 is enabled by the language mode, C++26, and not by -fcontracts:
// C++23 with -fcontracts has P2900's feature-test macro values, and a
// specifier naming `enforced' is pointed at -fcontracts-p4451.
// { dg-do compile }
// { dg-options "-std=c++23 -fcontracts" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

#if __cpp_contracts != 202502L || __cpp_lib_contracts != 202502L
#error "P2900's values"
#endif
#ifdef __gcc_contracts_p4451
#error "__gcc_contracts_p4451"
#endif

void f (int x) pre<std::contracts::controls::enforced> (x > 0);	// { dg-error "requires .-fcontracts-p4451." }
