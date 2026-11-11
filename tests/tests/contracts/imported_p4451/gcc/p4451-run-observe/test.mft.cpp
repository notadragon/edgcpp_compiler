//remark: imported from gcc:p4451-run-observe.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p4451 --contracts_p3097 --contracts_p3098 --contracts_p3099 --contracts_p4283 --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// p4451-run.C under -fcontracts-p4451 alone, with observe configured.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p4451 -fcontracts-p3097 -fcontracts-p3098 -fcontracts-p3099 -fcontracts-p4283" }
// { dg-additional-options "-fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include "p4451-run.C"
