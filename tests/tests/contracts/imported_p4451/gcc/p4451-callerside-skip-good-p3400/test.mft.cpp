//remark: imported from gcc:p4451-callerside-skip-good-p3400.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4451-callerside-skip-good.json
//options: --c++26 --contracts_p3400 --contract_configuration_file=p4451-callerside-skip-good.json
//use_system_includes: true
//linker_options: -lcontracts
// EDG: g++ makes the caller-side checks (through edg-gxx); the
// C-generating back end makes none, so this runs under edg_x86_64_gxx
// only.
// p4451-callerside-skip.C under -fcontracts-p3400 alone (the P3400
// label), with the caller-side checks of both preconditions asked for
// (p4451-callerside-skip-good.json).
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p3400" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4451-callerside-skip-good.json" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include "p4451-callerside-skip.C"
