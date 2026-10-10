//remark: imported from clang:explicit-spec-member-template-contracts.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 36: (?:catastrophic )?error
//match_regex: ", line 44: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

// An explicit specialization of a member template of a class template
// specialization is a function template of its own: its contracts are its
// first declaration's, not the primary member template's (CLANG-621: they
// were compared with the primary's), and its later redeclarations get the
// ordinary checks.
// (GCC mirror: g++.dg/contracts/cpp26/open-bug-member-template-spec-contracts.C,
// xfailed there as GCC-627.)

template<typename T>
struct A {
  template<typename P> int f(T t, P) pre(t > 0) { return 0; }
};

// Renamed parameter on a non-defining declaration.
template<>
template<typename Q>
int A<long>::f(long a, Q) pre(a > 3);
template<>
template<typename Q>
int A<long>::f(long a, Q) pre(a > 3) { return 2; }

// A redeclaration whose contract differs.
template<>
template<typename Q>
int A<char>::f(char t, Q) pre(t > 4); // expected-note {{contract previously specified with a non-equivalent condition}}
template<>
template<typename Q>
int A<char>::f(char t, Q) pre(t > 5) { return 4; } // expected-error {{method out-of-line definition differs in contract specifier sequence}} expected-note {{in contract specified here}}

// A redeclaration that adds contracts.
template<>
template<typename Q>
int A<unsigned>::f(unsigned t, Q); // expected-note {{previously declared without contracts here}}
template<>
template<typename Q>
int A<unsigned>::f(unsigned t, Q) pre(t > 6) { return 5; } // expected-error {{method out-of-line definition differs in contract specifier sequence}}

// Controls: its own contracts, different from the primary's, are accepted
// on a definition, as is a declaration without contracts.
template<>
template<typename Q>
int A<double>::f(double t, Q) pre(t > 2) { return 1; }
template<>
template<typename Q>
int A<short>::f(short t, Q);
template<>
template<typename Q>
int A<short>::f(short t, Q) { return 3; }

void use() {
  A<long>().f(1L, 0);
  A<double>().f(1.0, 0);
  A<short>().f(short(1), 0);
}
