//remark: imported from gcc:p3595-unsupported-criterion.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-unsupported-criterion.json
//options: --c++26 --contracts --contract_configuration_file=p3595-unsupported-criterion.json
//use_system_includes: true
//linker_options: -lcontracts
// P3595 ("File Format"): a configuration entry with an unsupported match
// criterion is skipped, so the entry meant for another module does not turn
// f's precondition off: the violation is enforced and the handler, which
// exits 0, is reached.
//
// Mirror: clang/test/Contracts/p3595-unsupported-criterion.cpp in the
// llvm_llvm-project fork.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -Wno-contract-configuration" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p3595-unsupported-criterion.json" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
#include <cstdlib>

void handle_contract_violation (const std::contracts::contract_violation&)
{
  std::exit (0);   // the precondition was checked
}

int f (int x) pre (x > 0) { return x; }

int main ()
{
  f (-1);
  return 1;        // the entry for another module turned the check off
}
