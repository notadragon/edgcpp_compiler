//remark: imported from clang:virtual-contracts-require-p3097.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 14: (?:catastrophic )?error
// Base/extension partition: Clang routes contracts on virtual functions to
// P3097, so in base mode (without -fcontracts-p3097) a contract on a virtual
// function is rejected. This is a documented divergence from GCC, whose base
// facility accepts contracts on a virtual function's first declaration (and
// separately forbids *adding* them in an override; g++.dg/.../dcl.contract.func.p6.C).
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

struct B {
  virtual void f(int x) pre(x > 0); // expected-error {{contracts on virtual functions require '-fcontracts-p3097'}}
};

struct D : B {
  void f(int x) override; // no contract here: fine
};
