//remark: imported from clang:Contracts/p3098-unexpanded-pack-predicate.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098 --contract_evaluation_semantic=enforce
//match_regex: ", line 16: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3098 -fcontract-evaluation-semantic=enforce -fsyntax-only -verify %s

// A P3098 init-capture pack named unexpanded in the postcondition's
// predicate is diagnosed in the template definition; expanded in a fold, it
// is fine.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/p3098-unexpanded-pack-capture.C
// and p3098-unexpanded-pack-capture-inst.C.

template <class... T>
int p5(T... xs) post [...y = xs] (y > 0) { return 0; } // expected-error {{expression contains unexpanded parameter pack 'y'}}

template <class... T>
int ok(T... xs) post [...y = xs] (((y > 0) && ...)) { return 0; }

int main() { return ok(1, 2); }
