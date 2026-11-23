//remark: imported from clang:dynamic-selection-late-include.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: dynamic-selection-late-include.json
//options: --c++26 --contracts --contract_configuration_file=dynamic-selection-late-include.json
// RUN: %clangxx -std=c++26 -fcontracts -fcontract-configuration-file=%S/dynamic-selection-late-include.json %libcxx_flags -c %s -o %t.o

// Under P3595 dynamic selection, a function using contracts is completed
// before <contracts> is included; the header's evaluation_semantic is then
// accepted.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/dynamic-selection-late-include.C

int f(int x) pre(x > 0) { return x; }
int g(int x) { return f(x); }

#include <contracts>

int h() { return g(1); }

// REQUIRES: contracts-libcxx
