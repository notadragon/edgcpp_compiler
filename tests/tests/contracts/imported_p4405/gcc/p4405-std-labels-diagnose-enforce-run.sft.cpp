//remark: imported from gcc:p4405-std-labels-diagnose-enforce-run.C
//type: ra
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=enforce
//use_system_includes: true
//linker_options: -lcontracts
//match_regex: ^violation: .*, enforce$
//match_regex: Test_name.c:40:[0-9]+: warning: contract assertion cannot be proven \[-Wcontract-unproven\]
// EDG: adapted -- the comment is formed from the predicate cpfe-cp puts
// out (implicit conversions shown), which varies conformingly (DECISIONS.md
// E10), so only the semantic is matched; g++'s warnings are matched too.
// D4405 ("Controlling the Analysis Semantic with Labels"), E198 reading 1:
// the standard diagnose label sets only the analysis semantic, so built
// to enforce, pre<diagnose> warns at compile time that it cannot be proven
// and its runtime evaluation is still enforce: f(-1) is a violation, and
// the program terminates after the handler returns.
// (p4405-std-labels-diagnose-observe-run.C: built to observe.)
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405" }
// { dg-additional-options "-fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
// { dg-shouldfail "enforce terminates after the violation" }

#include <contracts>
#include <cstdio>

void handle_contract_violation (const std::contracts::contract_violation &v)
{
  std::printf ("violation: %s, %s\n", v.comment (),
	       v.semantic () == std::contracts::evaluation_semantic::enforce
	       ? "enforce" : "other");
  std::fflush (stdout);
}

[[gnu::noipa]] void f (int x) pre<diagnose> (x > 0) {}

int
main (int argc, char **)
{
  f (argc - 2);	// { dg-warning "contract assertion cannot be proven \\\[-Wcontract-unproven\\\]" }
  std::puts ("after");
}

// { dg-output "violation: x > 0, enforce" }
