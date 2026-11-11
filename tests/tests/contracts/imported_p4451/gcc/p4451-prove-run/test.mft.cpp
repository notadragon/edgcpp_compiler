//remark: imported from gcc:p4451-prove-run.C
//type: ra
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p4451 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=diagnose -O2
//use_system_includes: true
//linker_options: -lcontracts
//match_regex: ^violation: 
// EDG: adapted -- the comment is formed from the predicate cpfe-cp puts
// out (implicit conversions shown), which varies conformingly (DECISIONS.md
// E10), so only the violation's line is matched; the included test is
// copied beside it; -O2 as in the original.
// p4405-paper-analysis-terminating-run.C under -fcontracts-p4451 alone: the
// terminating precondition is still enforced when built to ignore contract
// assertions, and a configured analysis semantic of diagnose, which
// terminating does not allow, falls back to ignore.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p4451 -fcontracts-p4405 -O2" }
// { dg-additional-options "-fcontract-evaluation-semantic=ignore -fcontract-analysis-semantic=diagnose" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
// { dg-shouldfail "enforce terminates after the violation" }

#include "p4405-paper-analysis-terminating-run.C"

// { dg-output "violation: p >= 0 && p <= 100" }
