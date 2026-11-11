//remark: imported from gcc:p4405-prove-optimize-premises-1.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --contracts_prove_optimize=1 --contract_evaluation_semantic=observe -O2
//match_regex: Test_name.c:45:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405's proof optimization level (DECISIONS.md E162-E168, E205): the
// callee premise copies (-fcontracts-prove-callee-premises) are on from
// level 2.  In D4405R0's Peter Bindels `expected` example under observe
// (p4405-shadow-callee-premises-bindels-observe.C), the call of value ()
// in the else branch is proven only through index ()'s premise copy.
// Here -fcontracts-prove-optimize=1 at -O2: no premise copies.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O2 -fcontracts-prove-optimize=1" }
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
