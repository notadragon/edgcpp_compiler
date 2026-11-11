//remark: imported from gcc:p4298-predicate-throws.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4298 --contract_evaluation_semantic=noexcept_enforce
//use_system_includes: true
//linker_options: -lcontracts
// P4298: predicate-evaluation exceptions under noexcept_enforce still
// route through the terminating entry point for the handler call.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4298 -fcontract-evaluation-semantic=noexcept_enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
#include <contracts>
#include <exception>
#include <cstdlib>

bool throws_on_eval(int x) { if (x < 0) throw 1; return x > 0; }

void handle_contract_violation(const std::contracts::contract_violation& v)
{
  if (v.detection_mode() != std::contracts::detection_mode::evaluation_exception)
    __builtin_abort ();
  throw 2;  // the handler itself throws
}

int f(int x) pre(throws_on_eval(x)) { return x; }

int main()
{
  std::set_terminate([]() { std::exit(0); });
  f(-1);        // predicate throws -> handler called -> handler throws -> terminate
  __builtin_abort ();
}
