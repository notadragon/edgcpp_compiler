//remark: imported from gcc:p4405-paper-local-O0.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 -O0 --contracts_prove_optimize=0
//match_regex: Test_name.c:30:[0-9]+: error: 
//match_regex: Test_name.c:32:[0-9]+: error: 
//match_regex: Test_name.c:42:[0-9]+: error: 
//match_regex: Test_name.c:49:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0, "Local Proofs": at proof level 0 only folding proves, so
// every always_prove assertion of the paper's three examples other than
// that of f(1) is an error, and the always_diagnose one still warns.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O0 -fcontracts-prove-optimize=0" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;

namespace pre_example {
void f(int x) pre<always_prove>(x > 0);	// { dg-message "the predicate of the contract assertion" }

void g(int y)
{
  f(1);		// { dg-bogus "cannot be proven|always false" }
  f(y);		// { dg-error "contract assertion cannot be proven" }
  if (y > 0) {
    f(y);	// { dg-error "contract assertion cannot be proven" }
  }
}
}

namespace assert_example {
void f(int x)
{
  contract_assert<always_diagnose>(x > 0);	// { dg-warning "contract assertion cannot be proven" }
  if (x > 0) {
    contract_assert<always_prove>(x > 0);	// { dg-error "contract assertion cannot be proven" }
  }
}
}

namespace post_example {
int g(bool b)
  post<always_prove>(r : r > 0)	// { dg-error "contract assertion cannot be proven" }
{
  return b ? 17 : 34;
}
}
