//remark: imported from gcc:p4451-callerside-seq-outlined.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4451-callerside-seq.json
//options: --c++26 --contracts_p4451 -fcontract-checks-outlined --contract_configuration_file=p4451-callerside-seq.json
//use_system_includes: true
//linker_options: -lcontracts
// EDG: g++ makes the caller-side checks (through edg-gxx); the
// C-generating back end makes none, so this runs under edg_x86_64_gxx
// only.
// EDG: adapted -- -fcontract-checks-outlined is g++'s option (the front
// end has none); eccp-gxx passes it to g++ as it is.
// p4451-callerside-seq.C with the checks outlined
// (-fcontract-checks-outlined), under -fcontracts-p4451 alone: the
// caller-side checks are made by the caller's wrapper the same way.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p4451 -fcontract-checks-outlined" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4451-callerside-seq.json" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include "p4451-callerside-seq.C"
