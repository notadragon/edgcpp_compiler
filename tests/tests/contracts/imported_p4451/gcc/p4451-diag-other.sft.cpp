//remark: imported from gcc:p4451-diag-other.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p4451
//match_regex: ", line 38: (?:catastrophic )?error
//match_regex: ", line 40: (?:catastrophic )?error
//match_regex: ", line 42: (?:catastrophic )?error
//match_regex: ", line 44: (?:catastrophic )?error
//match_regex: ", line 45: (?:catastrophic )?error
//match_regex: ", line 48: (?:catastrophic )?error
//match_regex: ", line 49: (?:catastrophic )?error
//match_regex: ", line 50: (?:catastrophic )?error
//match_regex: ", line 52: (?:catastrophic )?error
//match_regex: ", line 53: (?:catastrophic )?error
//match_regex: ", line 54: (?:catastrophic )?error
//match_regex: ", line 55: (?:catastrophic )?error
//match_regex: ", line 57: (?:catastrophic )?error
//match_regex: ", line 58: (?:catastrophic )?error
// P4451 [basic.contract.control] (D4451R1): without -fcontracts-p3400 the
// assertion-control-specifier is `< id-expression >', and lookup for it
// must find the standard library declaration of
// std::contracts::controls::enforced.  Anything that is not an
// id-expression, an id-expression naming another entity (a copy of the
// label, a class member referring to it), and a name nothing declares --
// another name in std::contracts::controls, P3400's labels being absent --
// are each rejected once, with a note saying why.  <contracts> declares
// controls::enforced alone, with none of P3400's library.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p4451" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

constexpr auto t = std::contracts::controls::enforced;	// { dg-message "lookup of .t. finds" }
template <class> struct W { static constexpr auto &enforced = std::contracts::controls::enforced; };	// { dg-message "lookup of .enforced. finds" }
struct C { static constexpr auto &enforced = std::contracts::controls::enforced; };	// { dg-message "lookup of .enforced. finds" }

void f (int x) pre<other> (x > 0);	// { dg-error "assertion-control-specifier must name .std::contracts::controls::enforced." }
// { dg-message "unqualified lookup of .other. finds no declaration" "" { target *-*-* } .-1 }
void g (int x) pre<(std::contracts::controls::enforced)> (x > 0);	// { dg-error "must name .std::contracts::controls::enforced." }
// { dg-message "the assertion-control-specifier is not an id-expression" "" { target *-*-* } .-1 }
void h (int x) { contract_assert<(enforced)> (x > 0); }	// { dg-error "must name .std::contracts::controls::enforced." }
// { dg-message "not an id-expression" "" { target *-*-* } .-1 }
void i (int x) pre<t> (x > 0);	// { dg-error "must name .std::contracts::controls::enforced." }
void j (int x) pre<std::contracts::controls::enforce> (x > 0);	// { dg-error "must name .std::contracts::controls::enforced." }
// { dg-message "lookup of .enforce. in .std::contracts::controls. finds no declaration" "" { target *-*-* } .-1 }
void k (int x) pre<std::contracts::controls::enforced> (x > 0);
void l (int x) pre<W<int>::enforced> (x > 0);	// { dg-error "must name .std::contracts::controls::enforced." }
void m (int x) pre<C::enforced> (x > 0);	// { dg-error "must name .std::contracts::controls::enforced." }
void n (int x) pre<::enforced> (x > 0);	// { dg-error "must name .std::contracts::controls::enforced." }
// { dg-message "lookup of .enforced. in .* finds no declaration" "" { target *-*-* } .-1 }
void o (int x) pre<std::contracts::controls::enforced + 0> (x > 0);	// { dg-error "must name .std::contracts::controls::enforced." }
void p (int x) pre<std::contracts::controls::enforced()> (x > 0);	// { dg-error "must name .std::contracts::controls::enforced." }
void q (int x) pre<1> (x > 0);	// { dg-error "must name .std::contracts::controls::enforced." }
void r (int x) pre<C::enforced> (x > 0) pre<std::contracts::controls::enforced> (x > 1);	// { dg-error "must name .std::contracts::controls::enforced." }

auto e = std::contracts::controls::empty_label;	// { dg-error "empty_label" }
auto a = std::contracts::controls::always_enforce;	// { dg-error "always_enforce" }

#ifdef __cpp_lib_contracts_labels
#error "no P3400 library"
#endif
#ifdef __cpp_contracts_labels
#error "no P3400"
#endif
