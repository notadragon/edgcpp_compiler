//remark: imported from gcc:p4451-callerside-client-check-p3400.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options_sep: !
//options: --c++26 --contracts_p3400 '--contract_configuration=[{"match":{"caller":true,"kind":"pre"},"output":{"semantic":"enforce"}}]'
//use_system_includes: true
//linker_options: -lcontracts
// EDG: g++ makes the caller-side checks (through edg-gxx); the
// C-generating back end makes none, so this runs under edg_x86_64_gxx
// only.
// EDG: adapted -- the front end has no -fcontracts-client-check=pre;
// P3595's entry for it, a "caller" match of the kind "pre" with the
// semantic enforce, stands in for it.
// p4451-callerside-client-check.C under -fcontracts-p3400 alone (the
// P3400 label).
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p3400 -fcontracts-client-check=pre" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include "p4451-callerside-client-check.C"
