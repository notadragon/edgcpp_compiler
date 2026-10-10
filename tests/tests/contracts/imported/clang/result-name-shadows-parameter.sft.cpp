//remark: imported from clang:result-name-shadows-parameter.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=ignore
//match_regex: ", line 16: (?:catastrophic )?error
//match_regex: ", line 17: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontract-evaluation-semantic=ignore -fsyntax-only -verify %s

// A postcondition result name that conflicts with a parameter is ill-formed
// ([basic.scope.contract]) whatever the evaluation semantic and whether or not
// the declaration is a definition.  The lambda-capture case, which Clang
// misses, is in OpenBugs/result-name-shadows-lambda-capture.cpp.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/result-name-shadows-parameter.C

int f(int r) post(r : r > 0) { return r; } // expected-error {{declaration of result name 'r' shadows parameter}} expected-note {{previous declaration is here}}
int g(int r) post(r : r > 0);              // expected-error {{declaration of result name 'r' shadows parameter}} expected-note {{previous declaration is here}}
