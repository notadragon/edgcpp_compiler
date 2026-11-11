//remark: imported from gcc:p4405-nested-side-branch.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4405-nested-diagnose.json
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=ignore --contract_configuration_file=p4405-nested-diagnose.json --contract_analysis_semantic=prove -fcontracts-prove-assume -O2:--c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=ignore --contract_configuration_file=p4405-nested-diagnose.json --contract_analysis_semantic=prove -fcontracts-prove-assume -O2 -O0
//match_regex: Test_name.c:42:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405, DECISIONS.md E169 (12): a nested obligation's region is a side
// branch of the path that never joins it again, so its failure is no
// premise for the other proofs in the same shadow: `y >= 0' after the
// guard (line 36, obligation 2) is proven soundly, and assumed under
// -fcontracts-prove-assume, next to g1's failed nested precondition
// (line 33), whose outer assertion is unproven.  (Before PC4-GCC every
// surviving marker made the shadow's other proofs conditional.)  E169
// (10) is kept: a nested diagnose precondition not proven (dd::g5's, line
// 45) makes its own outer (obligation 3) conditional, so not assumed,
// while the outer's own result and the proof after it (line 46,
// obligation 5) stay sound.
//
// Mirror: clang/test/Contracts/p4405-nested-side-branch.cpp in the
// llvm_llvm-project fork (-Rcontract-proof for the dumps).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O2" }
// { dg-additional-options "-fcontract-evaluation-semantic=ignore" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4405-nested-diagnose.json" }
// { dg-additional-options "-fcontract-analysis-semantic=prove" }
// { dg-additional-options "-fcontracts-prove-assume  " }

int g1 (int x) pre (x != 0);	// { dg-message "precondition of a call in the predicate cannot be proven" }
int g1 (int) { return 1; }
namespace dd {
int g5 (int x) pre (x != 0);	// { dg-message "the predicate of the contract assertion" }
int g5 (int) { return 1; }
}
int sink (int);

int
side (int y)
{
  contract_assert (g1 (y) == 1);	// { dg-error "contract assertion cannot be proven" }
  if (y < 0)
    return 0;
  contract_assert (y >= 0);
  return sink (y);
}

int
side_diagnose (int y)
{
  if (y < 0)
    return 0;
  contract_assert (dd::g5 (y) == 1);	// { dg-warning "contract assertion cannot be proven" }
  contract_assert (y >= 0);
  return sink (y);
}

//
//
//
//
//
//
//
//

// The same diagnostics in a -O0 build, which proves at the proof level too
// (its shadows find these proofs conditional, not sound: -O0 computes no
// side-effect flags for the real callees, and drops the assumptions).
//
