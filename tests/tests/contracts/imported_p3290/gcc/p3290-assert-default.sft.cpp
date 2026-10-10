//remark: imported from gcc:p3290-assert-default.C
//type: ra
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3290
//use_system_includes: true
//linker_options: -lcontracts
//match_regex: 1 == 2
// P3290: assert macro unchanged without __STDC_WANT_ASSERT_USES_CONTRACTS__.
// A failing assert is the platform's: it aborts with the platform's message
// and never reaches the contract-violation handler, which here exits
// normally -- the failure dg-shouldfail would report.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p3290" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
// { dg-shouldfail "the platform's assert aborts" }
#include <cassert>
#include <contracts>
#include <cstdlib>
void handle_contract_violation (const std::contracts::contract_violation &)
{
  std::_Exit (0);
}
int main() {
  assert(1 == 1);
  assert(1 == 2);
  std::_Exit (0);
}
// { dg-output "1 == 2" }
