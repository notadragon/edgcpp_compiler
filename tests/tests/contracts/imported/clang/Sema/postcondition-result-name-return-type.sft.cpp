//remark: imported from clang:Sema/postcondition-result-name-return-type.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s
// expected-no-diagnostics

// A postcondition result name on a function template, explicitly
// instantiated, with a declared and a deduced return type, and on a function
// with a deduced return type.

template <class T>
T baz(const T x) post(r : r != 42) {
return x != 42;
}
template int baz(int);


template <class T>
auto bar(T x) post(r : r != 42) {
return x;
}
template auto bar(int);

auto foo(const int x) post(r : r != x) { return x; }
