//remark: imported from clang:Runnable/p4283-label-requires.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4283 --contract_evaluation_semantic=ignore
//use_system_includes: true
//linker_options: -lcontracts
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3400 -fcontracts-p4283 \
// RUN:   -fcontract-evaluation-semantic=ignore %libcxx_flags -o %t
// RUN: %t

// a contract carrying both a P3400 label and a P4283 requires-clause on a
// templated function -- the two features compose.  When the requires-clause is
// satisfied the labeled contract is active (its label routes to observe,
// overriding the configured ignore); when unsatisfied the whole contract (label
// included) is discarded.
// (GCC mirror: g++.dg/contracts/cpp26/p4283-label-requires.C.)

#include <contracts>
#include <concepts>
using std::contracts::evaluation_semantic;

template <evaluation_semantic _S>
struct fixed_t {
  using assertion_control_object = fixed_t;
  constexpr evaluation_semantic compute_semantic(evaluation_semantic) const {
    return _S;
  }
};
constexpr fixed_t<evaluation_semantic::observe> observe_lbl{};

static int viol = 0;
void handle_contract_violation(const std::contracts::contract_violation &) {
  ++viol;
}

// requires satisfied for integral T -> labeled contract active (observe);
// unsatisfied otherwise -> the whole contract is discarded.
template <class T>
T f(T x) pre<observe_lbl> requires (std::integral<T>) (x > 0) { return x; }

int main() {
  viol = 0;
  f<int>(-1);       // integral: contract active, observe -> one violation
  if (viol != 1) __builtin_abort();

  viol = 0;
  f<double>(-1.0);  // non-integral: contract discarded, nothing evaluated
  if (viol != 0) __builtin_abort();
}
