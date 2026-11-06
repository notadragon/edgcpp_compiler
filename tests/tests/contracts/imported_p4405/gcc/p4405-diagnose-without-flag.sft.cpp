//remark: imported from gcc:p4405-diagnose-without-flag.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=diagnose --no_warnings
//use_system_includes: true
//linker_options: -lcontracts
// EDG: adapted -- --no_warnings (-w): without a location g++'s warning
// names edg-gxx's temporary file; the front end's warning is tested by
// p4405/option.  The run is the point here.
// P4405: likewise for the diagnose semantic: without -fcontracts-p4405 it
// is not allowed, the compiler says so, and ignore is used instead.
//
// Mirror: clang/test/Contracts/p4405-diagnose-without-flag.cpp in the
// llvm_llvm-project fork.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=diagnose" }
// { dg-warning "requires .-fcontracts-p4405.; .ignore. is used instead" "" { target *-*-* } 0 }

static int evaluated;
bool chk (int x) { ++evaluated; return x > 0; }

int f (int x) pre (chk (x)) post (r: chk (r)) { contract_assert (chk (x)); return x; }

int main ()
{
  f (-1);
  return evaluated;
}
