//remark: imported from gcc:p3098-except-init-enforce.C
//type: ra
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098 --contract_evaluation_semantic=enforce
//use_system_includes: true
//linker_options: -lcontracts
//match_regex: contract violation in function int f.int. at .*(\n|\r\n|\r)
//match_regex: .assertion_kind: post_capture, semantic: enforce, mode: evaluation_exception.*(\n|\r\n|\r)
// P3098: Capture init exception, enforce -- handler runs, then terminate.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098 -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
// { dg-shouldfail "contract violation" }

struct Bad {
  Bad(int) { throw 42; }
  ~Bad() {}
};

int f(int i)
  post [b = Bad(1)] (true)
{
  return i;
}

int main() {
  f(10);
  // Should not reach here -- enforce terminates after handler.
  return 1;
}
// { dg-output "contract violation in function int f.int. at .*(\n|\r\n|\r)" }
// { dg-output ".assertion_kind: post_capture, semantic: enforce, mode: evaluation_exception.*(\n|\r\n|\r)" }
