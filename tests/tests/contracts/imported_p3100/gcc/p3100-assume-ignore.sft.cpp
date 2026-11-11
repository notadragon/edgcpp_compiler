//remark: imported from gcc:p3100-assume-ignore.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3100 --contract_evaluation_semantic=assume
//use_system_includes: true
//linker_options: -lcontracts
// P3100: selecting the "assume" semantic without -fcontracts-allow-assume
// resolves to "ignore" -- the predicate is not evaluated and no violation
// is reported, so a failing precondition is a no-op.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3100 -fcontract-evaluation-semantic=assume" }

int side_effect = 0;

bool check(int i) { ++side_effect; return i > 0; }

int f(int i) pre(check(i)) { return i; }

int main()
{
  f(-1);                     // would violate if enforced
  if (side_effect != 0)      // "ignore" must not evaluate the predicate
    __builtin_abort();
  return 0;
}
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
