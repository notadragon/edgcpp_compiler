//remark: imported from gcc:p4405-shadow-callee-premises-bindels-ignore.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore -O2:--c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore -O2 -O0
//match_regex: Test_name.c:53:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0's Peter Bindels `expected` example (the celink
// D4405R0-implexample-bindels-expected, with the paper's `proven` label
// spelled as the standard `prove`), D4405R0 Section 4.5, notadragon_wg21
// DECISIONS.md E196 item 2, E199 (b).  The call of value () in the else
// branch relies on the postcondition of index (): proven soundly when that
// is checked and terminating (enforce, quick_enforce), and conditionally,
// so not diagnosed, when it is observed -- through index ()'s premise copy
// (-fcontracts-prove-callee-premises, implied by -fcontracts-p4405).  The
// if branch needs no premise.
// Here with ignore: an ignore postcondition leaves nothing in index ()
// (GCC-885), so the else branch is still an error.
//
// Mirror: clang/test/Contracts/p4405-shadow-callee-premises-bindels.cpp in
// the llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O2" }
// { dg-additional-options "-fcontract-evaluation-semantic=ignore" }

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
      use (p.value ());	// { dg-error "cannot be proven" }
    }
}
