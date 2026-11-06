//remark: imported from gcc:p4283-redecl.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4283
//match_regex: ", line 28: (?:catastrophic )?error
// P4283: Redeclaration matching for requires clauses.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4283" }

#include <concepts>

// Matching requires clauses: OK.
template <typename T>
void f(T x)
  pre requires(std::integral<T>) (x > 0);

template <typename T>
void f(T x)
  pre requires(std::integral<T>) (x > 0);

// Mismatched requires clauses: error.
template <typename T>
void g(T x)
  pre requires(std::integral<T>) (x > 0);

template <typename T>
void g(T x)
  pre requires(std::floating_point<T>) (x > 0);  // { dg-error "mismatched" }
