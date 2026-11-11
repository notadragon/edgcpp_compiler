//remark: imported from gcc:p3099-message-nullptr.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3099 --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// P3099: Verify message() returns nullptr when no message supplied.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p3099 -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

void f(int x) pre(x > 0) { }

static bool handler_called = false;

void handle_contract_violation(const std::contracts::contract_violation& v) {
  handler_called = true;
  // message() must return nullptr when no message was supplied.
  if (v.message() != nullptr)
    __builtin_abort();
  // comment() should still work.
  if (v.comment() == nullptr)
    __builtin_abort();
}

int main() {
  f(-1);  // triggers violation, observe semantic continues
  if (!handler_called)
    __builtin_abort();
}
