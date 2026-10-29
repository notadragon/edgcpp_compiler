//remark: imported from clang:postcondition-unnamed-param-definition.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=enforce
//match_regex: ", line 30: (?:catastrophic )?error
//match_regex: ", line 36: (?:catastrophic )?error
//match_regex: ", line 40: (?:catastrophic )?error
//match_regex: ", line 45: (?:catastrophic )?error
//match_regex: ", line 52: (?:catastrophic )?error
//match_regex: ", line 57: (?:catastrophic )?error
// EDG: adapted -- each error is at the definition's unnamed parameter, as
// GCC reports it; Clang reports it at the postcondition, with a note on the
// definition.
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontract-evaluation-semantic=enforce -fcxx-exceptions -fexceptions -fsyntax-only -verify %s

// A definition that leaves unnamed a parameter that a postcondition on
// another declaration odr-uses must still declare it const
// ([dcl.contract.func]); the error names the parameter at the postcondition,
// with a note on the definition's unnamed parameter.
// postcondition-unnamed-param-diagnostic.cpp has the non-defining
// redeclaration and the function template; this adds the other kinds of
// definition.  Mirrored from the EDG front end, which missed them (EDG-107
// in the edgcpp_compiler fork).
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/postcondition-unnamed-param-definition.C
// in the gnu_gcc fork; tests/tests/contracts/sema/postcondition_unnamed_def_params.sft.cpp
// in the edgcpp_compiler fork.

void r1(const int b) post(b > 0); // Clang's error
void r1(int) {}                   // expected-error {{an unnamed value parameter used in a postcondition must be declared const}}
void r2(const int b) post(b > 0);
void r2(const int) {}             // OK

// Only the second parameter is odr-used.
void r3(const int a, const int b) post(b > 0); // Clang's error
void r3(int, int) {}                           // expected-error {{an unnamed value parameter used in a postcondition must be declared const}}

// A function-try-block.
void r4(const int b) post(b > 0);  // Clang's error
void r4(int) try {} catch (...) {} // expected-error {{an unnamed value parameter used in a postcondition must be declared const}}

// A function template whose parameter type is not dependent: diagnosed on
// the template definition, with no instantiation.
template <class T> void t2(const int b) post(b > 0); // Clang's error
template <class T> void t2(int) {}                   // expected-error {{an unnamed value parameter used in a postcondition must be declared const}}

// Member functions defined outside the class.
struct S {
  void m(const int b) post(b > 0); // Clang's error
  static void s(const int b) post(b > 0);
};
void S::m(int) {}       // expected-error {{an unnamed value parameter used in a postcondition must be declared const}}
void S::s(const int) {} // OK

// A postcondition with a result name.
int r5(const int b) post(r : r > b); // Clang's error
int r5(int) { return 1; }            // expected-error {{an unnamed value parameter used in a postcondition must be declared const}}

// An instance whose parameter type is const.
template <class T> void t3(const T b) post(b > 0);
template <class T> void t3(T) {} // OK
template void t3<const int>(int);
