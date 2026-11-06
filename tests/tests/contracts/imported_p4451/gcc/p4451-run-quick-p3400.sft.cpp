//remark: imported from gcc:p4451-run-quick-p3400.C
//type: ra
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400 --contract_evaluation_semantic=quick_enforce
//use_system_includes: true
//linker_options: -lcontracts
// P4451: a configured quick_enforce is a terminating semantic and is kept
// for a terminating contract assertion: the violation traps without calling
// the handler, which would exit successfully.  Under -fcontracts-p3400 alone (the P3400 label).
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p3400 -fcontract-evaluation-semantic=quick_enforce" }
// { dg-shouldfail "quick_enforce traps" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
#include <cstdlib>

void handle_contract_violation (const std::contracts::contract_violation &)
{
  std::_Exit (0);
}

int f (int x) pre<terminating> (x > 0) { return x; }

int
main ()
{
  return f (-1);
}
