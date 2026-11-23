//remark: imported from gcc:p3595-dynamic-constexpr-skip.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-dynamic-constexpr-skip.json
//options: --c++26 --contracts --contracts_p3400 --contract_configuration_file=p3595-dynamic-constexpr-skip.json
//match_regex: ", line 13: (?:catastrophic )?error
// P3595: a dynamic entry with NO "semantic" is skipped in constant evaluation;
// scanning falls through to the next entry (here: enforce).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p3595-dynamic-constexpr-skip.json" }
#include <contracts>
constexpr int g(int x) pre(x > 0) { return x; } // { dg-error "contract predicate is false in constant expression" }
// In constant evaluation, entry 1 (dynamic, no semantic) is skipped, so entry 2
// (enforce) applies -> a false predicate is a compile-time error.  If the skip
// rule were broken, entry 1 would match and resolution would hit CES_INVALID,
// emitting "no valid evaluation semantic" instead; matching the enforce text
// proves the fall-through to entry 2 happened.
constexpr int good = g(1);   // no error
constexpr int bad  = g(-1);
