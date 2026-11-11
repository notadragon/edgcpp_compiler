//remark: imported from gcc:p4405-prove-optimize-bindels-O1-no-premises.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --no_contracts_prove_callee_premises --contract_evaluation_semantic=observe -O1
//match_regex: Test_name.c:48:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405's proof optimization level (DECISIONS.md E162-E168, E206): D4405R0's
// Peter Bindels `expected` example (p4405-shadow-callee-premises-bindels.C)
// with observe in a -O1 build, at the default proof level, 2, without the
// callee premise copies: the call of error () is proven through the level
// copies of the -O1 callees; the call of value () needs index ()'s premise
// copy.
//
// Mirror: clang/test/Contracts/p4405-prove-optimize-bindels.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O1 -fno-contracts-prove-callee-premises" }
// { dg-additional-options "-fcontract-evaluation-semantic=observe" }

#include <contracts>
using namespace std::contracts::labels;

struct Expected {
  int d_value;
  int d_error;
  int d_index;

  int index () const post (r : r == 0 || r == 1) { return d_index; }
  bool is_error () const { return index () == 1; }
  int error () const pre<prove> (index () == 1) { return d_error; }
  int value () const pre<prove> (index () == 0) { return d_value; }	// { dg-message "the predicate of the contract assertion" }
};

Expected get12thPrime ();
void use (int);

void g ()
{
  Expected p = get12thPrime ();
  if (p.is_error ())
    {
      use (p.error ());	// { dg-bogus "cannot be proven" }
    }
  else
    {
      use (p.value ());	// { dg-error "contract assertion cannot be proven" }
    }
}
