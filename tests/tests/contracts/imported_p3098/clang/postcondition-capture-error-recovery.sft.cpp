//remark: imported from clang:postcondition-capture-error-recovery.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098
//match_regex: ", line 15: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3098 -fsyntax-only -verify %s

// After a malformed postcondition capture such as `post[1]', the capture
// parser recovers past the `]' and the predicate is still parsed in the
// parameter scope: no spurious "use of undeclared identifier 'x'".  (The
// parameter is const so that naming it in the postcondition is valid.)
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/postcondition-capture-error-recovery.C

int k(const int x) post[1](x > 0); // expected-error {{expected identifier}}
