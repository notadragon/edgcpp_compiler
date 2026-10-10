//remark: imported from clang:Contracts/postcondition-unnamed-param-diagnostic.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 22: (?:catastrophic )?error
// EDG: adapted -- the function template shape is left out: its definition,
// which leaves the parameter unnamed and not const, is not diagnosed
// (EDG-107, watch test openbugs/edg-107-unnamed-param-template-definition).
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

// When a declaration leaves unnamed a parameter that a postcondition on
// another declaration odr-uses, and does not declare it const, the error
// names the parameter as the postcondition does (CLANG-640: it printed
// "parameter (null)"), for a later declaration of a function and for the
// definition of a function template.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/postcondition-unnamed-param-redecl.C
// in the gnu_gcc fork.  Found by the EDG fork's
// sema/postcondition_params_redecl test.

void r(const int b) post(b > 0); // expected-note {{odr-used in a postcondition here}}
void r(int);                     // expected-error {{parameter 'b' referenced in contract postcondition must be declared const}}

// EDG: the function template's definition is not diagnosed (EDG-107).
