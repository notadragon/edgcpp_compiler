//remark: imported from gcc:p4405-analysis-system-header-sys.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4405-analysis-system-header.h
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 -O0 -Wsystem-headers -Wno-literal-suffix -Wno-pedantic
//use_system_includes: true
//match_regex: p4405-analysis-system-header\.h:20:[0-9]+: warning: contract assertion cannot be proven \[-Wcontract-unproven\]
//match_regex: p4405-analysis-system-header\.h:32:[0-9]+: warning: contract assertion cannot be proven \[-Wcontract-unproven\]
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations, so the test compiles (type cp) and
// matches g++'s diagnostics; the header's "#pragma GCC system_header"
// reaches g++ as the system-header flag of cpfe-cp's line directives.
// p4405-analysis-system-header.C with -Wsystem-headers: the warnings for
// the assertions that are themselves in the system header, hidden there,
// are shown -- the header's own closure analyzed at sys_invoke's call (line
// 20 of the header, with the program's closure) and sys_callee's
// precondition analyzed at sys_caller's call (line 32) -- so that test's
// silence is the system-header rule's, not a proof.  -Wno-literal-suffix and
// -Wno-pedantic keep the library's own extensions quiet.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O0 -Wsystem-headers -Wno-literal-suffix -Wno-pedantic" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include "p4405-analysis-system-header.h"

struct always_diagnose_t {
  using assertion_control_object = always_diagnose_t;
  static constexpr std::contracts::evaluation_semantic_set
    allowed_semantics{std::contracts::evaluation_semantic::diagnose};
};
constexpr always_diagnose_t always_diagnose{};

void
use (int n)
{
  auto ld = [] (int x) pre<always_diagnose> (x > 0) {};	// { dg-message "the predicate of the contract assertion" }
  sys_invoke (ld, n);				// { dg-message "required from here" }
  sys_caller (n);				// { dg-message "required from here" }
  sys_lambda_caller (n);
  sys_callee (n);				// { dg-warning "contract assertion cannot be proven" }
}

// { dg-regexp {[^\n]*p4405-analysis-system-header\.h:20:[0-9]+: warning: contract assertion cannot be proven \[-Wcontract-unproven\]\n} }
// { dg-regexp {[^\n]*p4405-analysis-system-header\.h:20:[0-9]+: warning: contract assertion cannot be proven \[-Wcontract-unproven\]\n} }
// { dg-regexp {[^\n]*p4405-analysis-system-header\.h:32:[0-9]+: warning: contract assertion cannot be proven \[-Wcontract-unproven\]\n} }
// { dg-message "the predicate of the contract assertion" "" { target *-*-* } 0 }
// { dg-message "In instantiation of" "" { target *-*-* } 0 }
