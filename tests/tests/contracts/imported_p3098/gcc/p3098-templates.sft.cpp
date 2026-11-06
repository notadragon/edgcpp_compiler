//remark: imported from gcc:p3098-templates.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098 --contract_evaluation_semantic=enforce
//use_system_includes: true
//linker_options: -lcontracts
// P3098: Postcondition captures in function templates.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098 -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <cassert>

template <typename T>
T increment(T x)
  post [old_x = x] (r: r > old_x)
{
  return x + 1;
}

template <typename T>
T add(T a, T b)
  post [a, b] (r: r == a + b)
{
  T result = a + b;
  a = T{};
  b = T{};
  return result;
}

int main() {
  assert(increment(5) == 6);
  assert(increment(3.14) > 3.14);
  assert(add(3, 4) == 7);
  assert(add(1.5, 2.5) == 4.0);
}
