//remark: imported from gcc:p4405-analysis-flag-without-flag.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove --no_warnings
// EDG: adapted -- --no_warnings (-w): without a location g++'s warning
// names edg-gxx's temporary file; the front end's warning is tested by
// p4405/analysis_config.  That the test compiles is the point here.
// D4405: -fcontract-analysis-semantic=prove without -fcontracts-p4405 is not
// allowed; the compiler says so and the analysis evaluation is ignore, so
// nothing is diagnosed.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=ignore -fcontract-analysis-semantic=prove" }
// { dg-warning "'-fcontract-analysis-semantic=prove' requires '-fcontracts-p4405'; 'ignore' is used instead" "" { target *-*-* } 0 }

int f (int x)
  pre (x > 0)			// { dg-bogus "cannot be proven" }
{
  contract_assert (x != 3);	// { dg-bogus "cannot be proven" }
  return x;
}
