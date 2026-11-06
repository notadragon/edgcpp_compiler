//remark: imported from gcc:debug-and-opt.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// Check that we do not ICE with debug + optimisation.
// { dg-do run { target c++23 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=observe -O -g" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

int foo (const int i)
  pre (i > 3)
{
  return i;
}

int main()
{
  foo (1);
}
