//remark: imported from gcc:p3400-group-semantic-comma-list.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options_sep: !
//options: --c++26 --contracts --contracts_p3400 --contract_group_evaluation_semantic=a:observe,b:ignore
//use_system_includes: true
//linker_options: -lcontracts
// -fcontract-group-evaluation-semantic= accepts a comma-separated list of
// group:semantic elements: group "a" observes, group "b" is ignored.
//
// Mirror: clang/test/Contracts/group-semantic-comma-list.cpp in the
// llvm_llvm-project fork.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontract-group-evaluation-semantic=a:observe,b:ignore" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
using namespace std::contracts::labels;

static int violations = 0;
void handle_contract_violation (const std::contracts::contract_violation&)
{
  ++violations;
}

int f (int x) pre<"a.x"group> (x > 0) { return x; }
int g (int x) pre<"b.y"group> (x > 0) { return x; }

int main ()
{
  f (-1);
  g (-1);
  if (violations != 1)
    __builtin_abort ();
}
