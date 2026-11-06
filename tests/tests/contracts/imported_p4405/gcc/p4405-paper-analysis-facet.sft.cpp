//remark: imported from gcc:p4405-paper-analysis-facet.C
//type: ra
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=enforce -O2
//use_system_includes: true
//linker_options: -lcontracts
//match_regex: ^violation: 
// EDG: adapted -- the comment is formed from the predicate cpfe-cp puts
// out (implicit conversions shown), which varies conformingly (DECISIONS.md
// E10), so only the violation's line is matched.
// D4405R0, "P3400R5 Analysis Evaluation Configuration": the diagnosed label
// (an analysis_semantic facet) asks for the assertion to be diagnosed,
// leaving its runtime evaluations to the build: built to enforce, it warns
// that the assertion cannot be proven, and f(-1) is still a violation at
// run time.  The paper's example, with its comment
// (p4405-paper-analysis-facet-ignore.C and -prove.C vary the build).
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O2" }
// { dg-additional-options "-fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
// { dg-shouldfail "enforce terminates after the violation" }

#include <contracts>  // `evaluation_semantic`, `contract_violation`
#include <cstdio>     // `std::printf`, `std::fflush`, `std::puts`

// The paper's diagnosed label.
struct diagnosed_t {
  using assertion_control_object = diagnosed_t;

  constexpr std::contracts::evaluation_semantic analysis_semantic() const {
    return std::contracts::evaluation_semantic::diagnose;
  }
};
constexpr diagnosed_t diagnosed{};

// A violation handler that reports each violation; with *enforce* the
// program then terminates.
void handle_contract_violation(const std::contracts::contract_violation& v)
{
  std::printf("violation: %s\n", v.comment());
  std::fflush(stdout);
}

void f(int x)
{
  contract_assert<diagnosed>(x > 0);  // OK, warns: cannot be proven	{ dg-warning "contract assertion cannot be proven \\\[-Wcontract-unproven\\\]" }
}

int main()
{
  f(-1);  // Violation at runtime
  std::puts("done");
}

// { dg-output "violation: x > 0" }
