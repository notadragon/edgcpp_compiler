//remark: imported from gcc:p4405-analysis-address-wno.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_analysis_semantic=prove -Wno-contract-unproven
//match_regex: Test_name.c:41:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405: -Wno-contract-unproven silences the warning for taking the
// address of a function whose precondition allows only diagnose
// (always_diagnose), but not the error for one that allows only prove
// (always_prove; p4405-analysis-address.C).  A precondition that only the
// configuration analyzes is not diagnosed at all
// (p4405-analysis-address-flag.C).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -fcontract-analysis-semantic=prove -Wno-contract-unproven" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

struct always_prove_t {
  using assertion_control_object = always_prove_t;
  static constexpr std::contracts::evaluation_semantic_set
    allowed_semantics{std::contracts::evaluation_semantic::prove};
};
constexpr always_prove_t always_prove{};

struct always_diagnose_t {
  using assertion_control_object = always_diagnose_t;
  static constexpr std::contracts::evaluation_semantic_set
    allowed_semantics{std::contracts::evaluation_semantic::diagnose};
};
constexpr always_diagnose_t always_diagnose{};

int f (int x) pre (x > 0);
int fd (int x) pre<always_diagnose> (x > 0);	// { dg-bogus "precondition here" }
int fp (int x) pre<always_prove> (x > 0);	// { dg-message "precondition here" }

int (*p) (int) = f;				// { dg-bogus "address" }
int (*d) (int) = fd;				// { dg-bogus "address" }
int (*q) (int) = fp;				// { dg-error "contract assertion cannot be proven for a call through the address of 'int fp\\(int\\)'" }
