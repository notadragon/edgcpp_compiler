//remark: imported from clang:local-class-in-contract-lambda.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 20: (?:catastrophic )?error
//match_regex: ", line 21: (?:catastrophic )?error
//match_regex: ", line 24: (?:catastrophic )?error
//match_regex: ", line 28: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

// A member function of a local class inside a lambda in a contract names the
// enclosing function's parameter: ill-formed, as it is in a function body.
// A precondition is parsed off the declarator, before the parameters belong
// to the function, which used to let the reference through with no capture
// (and CodeGen then crashed once the member was called); in a contract_assert
// the contract scope walk asserted on meeting the local class.
//
// GCC: gcc/testsuite/g++.dg/contracts/cpp26/local-class-in-predicate-lambda.C

void f1(int x) pre([&] { struct Q { int m() { return x; } }; return true; }()); // expected-error {{reference to local variable 'x' declared in enclosing}} expected-note {{'x' declared here}}
void f2(int x) pre([&] { struct Q { int m() { return x; } }; return Q{}.m() > 0; }()) {} // expected-error {{reference to local variable 'x' declared in enclosing}} expected-note {{'x' declared here}}

void f3(int x) { // expected-note {{'x' declared here}}
  contract_assert([&] { struct Q { int m() { return x; } }; return true; }()); // expected-error {{reference to local variable 'x' declared in enclosing function}}
}

void f4(int x) { // expected-note {{'x' declared here}}
  contract_assert([&] { return [&] { struct Q { int m() { return x; } }; return true; }(); }()); // expected-error {{reference to local variable 'x' declared in enclosing function}}
}
