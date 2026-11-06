//remark: imported from gcc:p4451-run-dynamic-both.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4451-dynamic.json
//options: --c++26 --contracts_p3400 --contracts_p4451 --contract_configuration_file=p4451-dynamic.json
//use_system_includes: true
//linker_options: -lcontracts
// EDG: adapted -- the configuration file is copied beside the test and named
// without the DejaGnu ${srcdir} path.
// P4451: a semantic selected at run time (P3595 dynamic selection) is
// adjusted for a terminating contract assertion as for P3400's
// labels::terminating: a selected ignore or observe is evaluated as
// enforce.  Under -fcontracts-p3400 and -fcontracts-p4451 (the P3400 label).
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p3400 -fcontracts-p4451" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4451-dynamic.json" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
#include <cstdlib>

using std::contracts::evaluation_semantic;

static evaluation_semantic selected;

evaluation_semantic
p4451_sel ()
{
  return selected;
}

static int handled;

void handle_contract_violation (const std::contracts::contract_violation &v)
{
  if (v.semantic () != evaluation_semantic::enforce)
    std::abort ();
  ++handled;
  throw handled;
}

int f (int x) pre<terminating> (x > 0) { return x; }
void g (int x) { contract_assert<terminating> (x > 0); }

int
main ()
{
  for (evaluation_semantic s : { evaluation_semantic::ignore,
				 evaluation_semantic::observe,
				 evaluation_semantic::enforce })
    {
      selected = s;
      try { f (-1); std::abort (); } catch (int) { }
      try { g (-1); std::abort (); } catch (int) { }
    }
  if (handled != 6)
    std::abort ();
}
