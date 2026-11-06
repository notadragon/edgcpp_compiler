//remark: imported from gcc:p4405-paper-local-post.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 -O2
//use_system_includes: true
//linker_options: -lcontracts
//match_regex: 17 34
// D4405R0, "Local Proofs": a postcondition with always_prove is proven
// against the function body (by the proof shadow, optimized).  The paper's
// example, with its comment.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O2" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <cstdio>
#include <contracts>

using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;

// The paper's always_prove label, and always_diagnose, "defined the same
// way".
struct always_prove_t {
  using assertion_control_object = always_prove_t;
  static constexpr evaluation_semantic_set
    allowed_semantics = {evaluation_semantic::prove};
};
constexpr always_prove_t always_prove{};
struct always_diagnose_t {
  using assertion_control_object = always_diagnose_t;
  static constexpr evaluation_semantic_set
    allowed_semantics = {evaluation_semantic::diagnose};
};
constexpr always_diagnose_t always_diagnose{};

int g(bool b)
  post<always_prove>(r : r > 0)  // OK, proven	{ dg-bogus "cannot be proven|always false" }
{
  return b ? 17 : 34;
}

int main()
{
  std::printf("%d %d\n", g(true), g(false));
}

// { dg-output "17 34" }
