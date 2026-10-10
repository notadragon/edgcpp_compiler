//remark: imported from gcc:violation-handler-cv-param.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 19: (?:catastrophic )?error
// The contract-violation handler's parameter must be exactly "lvalue
// reference to const std::contracts::contract_violation"
// ([basic.contract.handler]), a diagnosable rule, so a reference to const
// volatile is rejected.  Stock GCC checks the handler's type not at all
// (GCC-59 in this repository's bug-reports/, fixed here).
//
// Mirror: clang/test/Contracts/violation-handler-cv-param.cpp.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }
// { dg-skip-if "requires hosted libstdc++" { ! hostedlib } }

#include <contracts>

void handle_contract_violation (const volatile std::contracts::contract_violation&) {} // { dg-error "handle_contract_violation" }
