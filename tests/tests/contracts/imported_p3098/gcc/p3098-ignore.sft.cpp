//remark: imported from gcc:p3098-ignore.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098 --contract_evaluation_semantic=ignore
//use_system_includes: true
//linker_options: -lcontracts
// P3098: Postcondition captures -- ignore semantic produces no side effects.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098 -fcontract-evaluation-semantic=ignore" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <cassert>

int construct_count = 0;
int destruct_count = 0;

struct Tracked {
  Tracked() { ++construct_count; }
  Tracked(const Tracked&) { ++construct_count; }
  Tracked(int) { ++construct_count; }
  ~Tracked() { ++destruct_count; }
};

int f(int i)
  post [t = Tracked(i)] (true)
{
  return i;
}

int main() {
  f(42);
  // With ignore semantic, nothing should be constructed or destroyed.
  assert(construct_count == 0);
  assert(destruct_count == 0);
}
