//remark: imported from gcc:explicit-spec-contract-mismatch.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 20: (?:catastrophic )?error
//match_regex: ", line 24: (?:catastrophic )?error
// An explicit specialization's redeclaration is compared with its first
// declaration like any redeclaration: a different contract, or one its first
// declaration lacks, is diagnosed.
//
// Clang: clang/test/Contracts/explicit-spec-contract-mismatch.cpp.
//
//
//
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

template <class T> void ft (T x);
template <> void ft<int> (int x) pre (x > 5);
template <> void ft<int> (int x) pre (x > 6) {} // { dg-error "mismatched contract" }

template <class T> void gt (T x);
template <> void gt<int> (int x);
template <> void gt<int> (int x) pre (x > 6) {} // { dg-error "declaration adds contracts" }
