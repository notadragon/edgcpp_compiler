//remark:contracts: contract assertions declared in a precompiled header, in constant evaluation and redeclarations
//type:fn
//source_files:header.h
//options_all:--pch --pch_test_mode --contract_evaluation_semantic=quick_enforce
//match_regex:using precompiled header file
//match_regex:"header.h", line 1: error: contract predicate is false in constant expression
//match_regex:"header.h", line 2: error: contract predicate is false in constant expression
//match_regex:"header.h", line 4: error: contract predicate is false in constant expression
//match_regex:"header.h", line 5: error: contract predicate is false in constant expression
//match_regex:"header.h", line 8: error: contract predicate is false in constant expression
//match_regex:line 30: error: mismatched contract condition in declaration
// The second compilation uses the PCH file the first one created.  ct is
// instantiated only after the PCH.  The header ends with a redeclaration of
// nf that omits its contracts, which is allowed; here one with different
// contracts is not.  Also runs under the C-generating back end.
#include "header.h"

static_assert(cf(1) == 1);        // OK
static_assert(cf(0) == 0);        // Error, precondition (line 1)
static_assert(ct(1) == 1);        // OK
static_assert(ct(0) == 0);        // Error, precondition (line 2)
static_assert(ct(9L) == 9);       // Error, postcondition (line 2)
static_assert(S(5).v == 5);       // OK
static_assert(S(0).v == 0);       // Error, constructor's precondition (line 4)
static_assert(S(5).mf(4) == 4);   // OK
static_assert(S(5).mf(6) == 6);   // Error, precondition (line 5)
static_assert(S(5).mf(2) == 2);   // Error, contract_assert (line 5)
static_assert(lam(1) == 1);       // OK
static_assert(lam(0) == 0);       // Error, the lambda's precondition (line 8)
int nf(int x) pre(x > 2);         // Error, different contracts
int nf(int x) pre(x > 1);         // OK
