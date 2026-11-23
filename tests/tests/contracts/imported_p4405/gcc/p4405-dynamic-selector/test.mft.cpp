//remark: imported from gcc:p4405-dynamic-selector.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4405-dynamic-selector.json
//options: --c++26 --contracts --contracts_p4405 --contract_configuration_file=p4405-dynamic-selector.json
//use_system_includes: true
//linker_options: -lcontracts
// P4405: a P3595 dynamic selector returning prove or diagnose behaves as
// ignore: the predicate is not evaluated, nothing is reported, and a
// dynamically selected contract is never diagnosed at compile time.  A
// checking value returned afterwards still checks.
//
// Mirror: clang/test/Contracts/p4405-dynamic-selector.cpp in the
// llvm_llvm-project fork.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4405-dynamic-selector.json" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

using std::contracts::evaluation_semantic;

static evaluation_semantic chosen;

extern "C" evaluation_semantic p4405_dynamic_sel () { return chosen; }

static int evaluated, violations;
bool chk (int x) { ++evaluated; return x > 0; }

void handle_contract_violation (const std::contracts::contract_violation &)
{
  ++violations;
}

int f (int x) pre (chk (x)) post (r: chk (r))
{
  contract_assert (chk (x));
  return x;
}

int main ()
{
  chosen = evaluation_semantic::prove;
  f (-1);
  chosen = evaluation_semantic::diagnose;
  f (-1);
  if (evaluated != 0 || violations != 0)
    __builtin_abort ();
  chosen = evaluation_semantic::observe;
  f (-1);
  if (evaluated != 3 || violations != 3)
    __builtin_abort ();
}
