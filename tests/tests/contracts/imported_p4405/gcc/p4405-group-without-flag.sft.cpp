//remark: imported from gcc:p4405-group-without-flag.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options_sep: !
//options: --c++26 --contracts --contracts_p3400 --contract_group_evaluation_semantic=g:prove,h:diagnose --no_warnings
//use_system_includes: true
//linker_options: -lcontracts
// EDG: adapted -- --no_warnings (-w): without a location g++'s warning
// names edg-gxx's temporary file; the front end's warning is tested by
// p4405/option.  The run is the point here.
// P4405: -fcontract-group-evaluation-semantic= asking for prove or diagnose
// without -fcontracts-p4405 is warned about (-Wcontract-configuration), and
// ignore is used instead.
//
// Mirror: clang/test/Contracts/p4405-group-without-flag.cpp in the
// llvm_llvm-project fork.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontract-group-evaluation-semantic=g:prove,h:diagnose" }
// { dg-warning "asks for .prove., which requires .-fcontracts-p4405.; .ignore. is used instead" "" { target *-*-* } 0 }
// { dg-warning "asks for .diagnose., which requires .-fcontracts-p4405.; .ignore. is used instead" "" { target *-*-* } 0 }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

static int evaluated;
bool chk (int x) { ++evaluated; return x > 0; }

int f (int x) pre<"g"group> (chk (x)) { return x; }
int g (int x) pre<"h"group> (chk (x)) { return x; }

int main ()
{
  f (-1);
  g (-1);
  return evaluated;
}
