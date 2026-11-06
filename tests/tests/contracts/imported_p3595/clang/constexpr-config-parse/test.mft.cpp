//remark: imported from clang:constexpr-config-parse.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: constexpr-config-parse.json
//options: --c++26 --contracts --contract_configuration_file=constexpr-config-parse.json
// Verify that "constexpr" key in JSON config parses without error and that
// runtime behavior uses the runtime entry (ignore).
// RUN: %clang_cc1 -std=c++26 -fcontracts \
// RUN:   -fcontract-configuration-file=%S/constexpr-config-parse.json \
// RUN:   -fsyntax-only -verify %s

// expected-no-diagnostics

constexpr int f(int x) pre(x >= 0) { return x; }

// Runtime semantic is ignore, so calling with invalid argument at runtime
// should not produce any diagnostic.  At CE time with enforce, this would
// be an error -- but this call is not in a manifestly-CE context.
int g() { return f(-1); }
