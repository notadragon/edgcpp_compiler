//remark: imported from clang:p3595-dynamic-parse.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-dynamic-parse.json
//options: --c++26 --contracts --contracts_p3400 --contract_configuration_file=p3595-dynamic-parse.json
// P3595: output.dynamic parses cleanly (no diagnostics).
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3400 -fcontract-configuration-file=%S/p3595-dynamic-parse.json -fsyntax-only -verify %s
// expected-no-diagnostics
void f(int x) pre(x > 0) { }
