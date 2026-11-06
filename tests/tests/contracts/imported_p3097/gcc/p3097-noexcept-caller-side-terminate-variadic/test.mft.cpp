//remark: imported from gcc:p3097-noexcept-caller-side-terminate-variadic.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contract_evaluation_semantic=enforce -DCASE=1
//use_system_includes: true
//linker_options: -lcontracts
// p3097-noexcept-caller-side-terminate.C for the C-variadic function.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontract-evaluation-semantic=enforce -DCASE=1" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include "p3097-noexcept-caller-side-terminate.C"
