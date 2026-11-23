//remark:contracts: twelve function templates declared in a precompiled header and instantiated after it keep their contract assertions in constant evaluation
//type:fn
//source_files:header.h
//options_all:--pch --pch_test_mode --contract_evaluation_semantic=quick_enforce
//match_regex:using precompiled header file
//match_regex:"header.h", line 1: error: contract predicate is false in constant expression
//match_regex:"header.h", line 2: error: contract predicate is false in constant expression
//match_regex:"header.h", line 3: error: contract predicate is false in constant expression
//match_regex:"header.h", line 4: error: contract predicate is false in constant expression
//match_regex:"header.h", line 5: error: contract predicate is false in constant expression
//match_regex:"header.h", line 6: error: contract predicate is false in constant expression
//match_regex:"header.h", line 7: error: contract predicate is false in constant expression
//match_regex:"header.h", line 8: error: contract predicate is false in constant expression
//match_regex:"header.h", line 9: error: contract predicate is false in constant expression
//match_regex:"header.h", line 10: error: contract predicate is false in constant expression
//match_regex:"header.h", line 11: error: contract predicate is false in constant expression
//match_regex:"header.h", line 12: error: contract predicate is false in constant expression
// Mirror of GCC's g++.dg/pch/contract-template-after-pch.C (GCC-646): the
// second compilation uses the PCH the first one created, and each template
// is instantiated only after it.  Twelve, so that a lookup that loses a
// template's contracts by chance cannot pass; each violates its
// precondition and its postcondition (header line i + 1).
#include "header.h"

static_assert(t0(1) == 1);
static_assert(t0(0) == 0);            // Error, precondition
static_assert(t0(9L) == 9);   // Error, postcondition
static_assert(t1(2) == 2);
static_assert(t1(0) == 0);            // Error, precondition
static_assert(t1(10L) == 10);   // Error, postcondition
static_assert(t2(3) == 3);
static_assert(t2(0) == 0);            // Error, precondition
static_assert(t2(11L) == 11);   // Error, postcondition
static_assert(t3(4) == 4);
static_assert(t3(0) == 0);            // Error, precondition
static_assert(t3(12L) == 12);   // Error, postcondition
static_assert(t4(5) == 5);
static_assert(t4(0) == 0);            // Error, precondition
static_assert(t4(13L) == 13);   // Error, postcondition
static_assert(t5(6) == 6);
static_assert(t5(0) == 0);            // Error, precondition
static_assert(t5(14L) == 14);   // Error, postcondition
static_assert(t6(7) == 7);
static_assert(t6(0) == 0);            // Error, precondition
static_assert(t6(15L) == 15);   // Error, postcondition
static_assert(t7(8) == 8);
static_assert(t7(0) == 0);            // Error, precondition
static_assert(t7(16L) == 16);   // Error, postcondition
static_assert(t8(9) == 9);
static_assert(t8(0) == 0);            // Error, precondition
static_assert(t8(17L) == 17);   // Error, postcondition
static_assert(t9(10) == 10);
static_assert(t9(0) == 0);            // Error, precondition
static_assert(t9(18L) == 18);   // Error, postcondition
static_assert(t10(11) == 11);
static_assert(t10(0) == 0);            // Error, precondition
static_assert(t10(19L) == 19);   // Error, postcondition
static_assert(t11(12) == 12);
static_assert(t11(0) == 0);            // Error, precondition
static_assert(t11(20L) == 20);   // Error, postcondition
