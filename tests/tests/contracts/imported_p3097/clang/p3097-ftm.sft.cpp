//remark: imported from clang:p3097-ftm.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3097 -fsyntax-only -verify %s
// expected-no-diagnostics

// P3097R3 bumps __cpp_contracts above 202502L.
#if __cpp_contracts <= 202502L
#error "__cpp_contracts not bumped above 202502L with -fcontracts-p3097"
#endif

struct Base {
  virtual void f() pre(true);
};

struct Derived : Base {
  void f() override pre(true);
};
