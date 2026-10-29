//remark: imported from gcc:p4405-analysis-json.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4405-analysis-json.json
//options: --c++26 --contracts --contracts_p4405 --contract_configuration_file=p4405-analysis-json.json
//match_regex: Test_name.c:26:[0-9]+: error: 
//match_regex: Test_name.c:34:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405 ("P3595R1 Analysis Evaluation Configuration"): entries with
// "analysis": true select the semantic of the analysis evaluation in a
// lookup of their own through the same list.  Only they match that lookup
// -- the first entry (no "analysis", kind pre, ignore) does not stop the
// precondition's analysis from being prove -- and the runtime lookups skip
// them.  An earlier entry excludes a namespace; "analysis" with
// "constexpr": true never matches (else every assertion would be proven),
// and with "constexpr": false it does; "analysis": false is the same as
// leaving the key out.  The runtime evaluations are ignore throughout.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4405-analysis-json.json" }

int f (int x)
  pre (x > 0)			// { dg-message "the predicate of the contract assertion" }
  post (r: r > 0)		// { dg-error "contract assertion cannot be proven" }
{
  contract_assert (x != 3);	// { dg-warning "contract assertion cannot be proven" }
  return x;
}

int use (int y)
{
  return f (y);			// { dg-error "contract assertion cannot be proven" }
}

namespace thirdparty {
int g (int x)
  pre (x > 0)			// { dg-bogus "cannot be proven" }
  post (r: r > 0)		// { dg-bogus "cannot be proven" }
{
  contract_assert (x != 3);	// { dg-bogus "cannot be proven" }
  return x;
}
}

int use_thirdparty (int y)
{
  return thirdparty::g (y);	// { dg-bogus "cannot be proven" }
}
