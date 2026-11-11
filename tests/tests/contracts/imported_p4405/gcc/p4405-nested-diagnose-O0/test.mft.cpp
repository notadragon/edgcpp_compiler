//remark: imported from gcc:p4405-nested-diagnose-O0.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4405-nested-diagnose.json
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove --contract_configuration_file=p4405-nested-diagnose.json -O0 --contracts_prove_optimize=0
//match_regex: Test_name.c:40:[0-9]+: error: 
//match_regex: Test_name.c:50:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// EDG: adapted -- the nested precondition's warning at its call, on the
// line after the assertion's in the original, is reported on the
// assertion's line: the C++-generating back end prints the statement on
// one line, under one #line directive.
// P4405, DECISIONS.md E169: p4405-nested-diagnose.C at proof level 0,
// folding alone: each nested diagnose precondition gets its own warning and the
// outer assertions, which folding cannot decide, are not proven -- without
// a note for the nested ones, which do not count for them.
//
// Mirror: clang/test/Contracts/p4405-nested-diagnose.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O0 -fcontracts-prove-optimize=0" }
// { dg-additional-options "-fcontract-evaluation-semantic=ignore" }
// { dg-additional-options "-fcontract-analysis-semantic=prove" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4405-nested-diagnose.json" }

namespace dd {
int g1 (int x) pre (x != 0);	// { dg-message "the predicate of the contract assertion" }
				// { dg-bogus "precondition of a call" "" { target *-*-* } .-1 }
int g1 (int) { return 1; }
int g2 (int x) pre (x != 0);	// { dg-message "the predicate of the contract assertion" }
				// { dg-bogus "precondition of a call" "" { target *-*-* } .-1 }
int g2 (int) { return 1; }
}

int
unprovable (int y)
{
  contract_assert (		// { dg-error "contract assertion cannot be proven" }
    dd::g1 (y) == 1);		// { dg-warning "contract assertion cannot be proven" }
  return y;
}

int
guarded (int y)
{
  if (y == 0)
    return 0;
  contract_assert (		// { dg-error "contract assertion cannot be proven" }
    dd::g2 (y) == 1);		// { dg-warning "contract assertion cannot be proven" }
  return y;
}
