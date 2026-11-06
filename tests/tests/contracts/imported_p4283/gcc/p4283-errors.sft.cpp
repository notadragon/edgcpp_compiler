//remark: imported from gcc:p4283-errors.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4283
//match_regex: ", line 15: (?:catastrophic )?error
//match_regex: ", line 18: (?:catastrophic )?error
// P4283: Error cases for requires clauses on contract assertions.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4283" }

#include <concepts>

// Error: requires on non-templated function.
void non_template(int x)
  pre requires(std::integral<int>) (x > 0);  // { dg-error "only allowed on templated functions" }

void non_template2(int x) {
  contract_assert requires(std::integral<int>) (x > 0);  // { dg-error "only allowed on templated functions" }
}
