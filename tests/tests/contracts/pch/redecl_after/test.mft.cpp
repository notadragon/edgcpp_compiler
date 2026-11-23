//remark:contracts: redeclarations after a precompiled header are matched against the contracts of twelve functions it declared
//type:fn
//source_files:header.h
//options_all:--pch --pch_test_mode --contract_evaluation_semantic=quick_enforce
//match_regex:using precompiled header file
//match_regex:line 25: error: mismatched contract condition in declaration
//match_regex:line 27: error: mismatched contract condition in declaration
//match_regex:line 29: error: mismatched contract condition in declaration
//match_regex:line 31: error: mismatched contract condition in declaration
//match_regex:line 33: error: mismatched contract condition in declaration
//match_regex:line 35: error: mismatched contract condition in declaration
//match_regex:line 37: error: mismatched contract condition in declaration
//match_regex:line 39: error: mismatched contract condition in declaration
//match_regex:line 41: error: mismatched contract condition in declaration
//match_regex:line 43: error: mismatched contract condition in declaration
//match_regex:line 45: error: mismatched contract condition in declaration
//match_regex:line 47: error: mismatched contract condition in declaration
// Mirror of GCC's g++.dg/pch/contract-redecl-after-pch.C (GCC-647): after
// the PCH, a redeclaration repeating a header function's contracts is
// valid and one with different contracts is not.  Twelve of each, so that
// a lookup that loses the header's contracts by chance cannot pass.
#include "header.h"

int nf0(int x) pre(x > 0);       // OK, the same contracts
int mf0(int x) pre(x > 1);       // Error, different contracts
int nf1(int x) pre(x > 1);       // OK, the same contracts
int mf1(int x) pre(x > 2);       // Error, different contracts
int nf2(int x) pre(x > 2);       // OK, the same contracts
int mf2(int x) pre(x > 3);       // Error, different contracts
int nf3(int x) pre(x > 3);       // OK, the same contracts
int mf3(int x) pre(x > 4);       // Error, different contracts
int nf4(int x) pre(x > 4);       // OK, the same contracts
int mf4(int x) pre(x > 5);       // Error, different contracts
int nf5(int x) pre(x > 5);       // OK, the same contracts
int mf5(int x) pre(x > 6);       // Error, different contracts
int nf6(int x) pre(x > 6);       // OK, the same contracts
int mf6(int x) pre(x > 7);       // Error, different contracts
int nf7(int x) pre(x > 7);       // OK, the same contracts
int mf7(int x) pre(x > 8);       // Error, different contracts
int nf8(int x) pre(x > 8);       // OK, the same contracts
int mf8(int x) pre(x > 9);       // Error, different contracts
int nf9(int x) pre(x > 9);       // OK, the same contracts
int mf9(int x) pre(x > 10);       // Error, different contracts
int nf10(int x) pre(x > 10);       // OK, the same contracts
int mf10(int x) pre(x > 11);       // Error, different contracts
int nf11(int x) pre(x > 11);       // OK, the same contracts
int mf11(int x) pre(x > 12);       // Error, different contracts
