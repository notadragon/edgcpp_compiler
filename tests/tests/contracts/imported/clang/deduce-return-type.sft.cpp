//remark: imported from clang:deduce-return-type.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -xc++ %s

auto foo(int x) -> int post(r : r != 42) { return x; } // OK
template <class T> auto bar(T x) -> T post(r : r != 42) { return x; }
template int bar(int);

auto bar(int x) post(r : r != 42) { return x; }
