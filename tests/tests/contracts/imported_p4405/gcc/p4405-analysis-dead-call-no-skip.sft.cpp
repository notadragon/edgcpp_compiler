//remark: imported from gcc:p4405-analysis-dead-call-no-skip.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --no_contract_analysis_skip_dead_calls --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove -O2
//match_regex: Test_name.c:72:[0-9]+: error: 
//match_regex: Test_name.c:73:[0-9]+: error: 
//match_regex: Test_name.c:74:[0-9]+: error: 
//match_regex: Test_name.c:75:[0-9]+: error: 
//match_regex: Test_name.c:76:[0-9]+: error: 
//match_regex: Test_name.c:77:[0-9]+: error: 
//match_regex: Test_name.c:78:[0-9]+: error: 
//match_regex: Test_name.c:79:[0-9]+: error: 
//match_regex: Test_name.c:80:[0-9]+: error: 
//match_regex: Test_name.c:81:[0-9]+: error: 
//match_regex: Test_name.c:82:[0-9]+: error: 
//match_regex: Test_name.c:83:[0-9]+: error: 
//match_regex: Test_name.c:84:[0-9]+: error: 
//match_regex: Test_name.c:85:[0-9]+: error: 
//match_regex: Test_name.c:86:[0-9]+: error: 
//match_regex: Test_name.c:89:[0-9]+: error: 
//match_regex: Test_name.c:90:[0-9]+: error: 
//match_regex: Test_name.c:91:[0-9]+: error: 
//match_regex: Test_name.c:92:[0-9]+: error: 
//match_regex: Test_name.c:93:[0-9]+: error: 
//match_regex: Test_name.c:94:[0-9]+: error: 
//match_regex: Test_name.c:95:[0-9]+: error: 
//match_regex: Test_name.c:96:[0-9]+: error: 
//match_regex: Test_name.c:100:[0-9]+: error: 
//match_regex: Test_name.c:105:[0-9]+: error: 
//match_regex: Test_name.c:106:[0-9]+: error: 
//match_regex: Test_name.c:107:[0-9]+: error: 
//match_regex: Test_name.c:108:[0-9]+: error: 
//match_regex: Test_name.c:109:[0-9]+: error: 
//match_regex: Test_name.c:110:[0-9]+: error: 
//match_regex: Test_name.c:111:[0-9]+: error: 
//match_regex: Test_name.c:112:[0-9]+: error: 
//match_regex: Test_name.c:114:[0-9]+: error: 
//match_regex: Test_name.c:115:[0-9]+: error: 
//match_regex: Test_name.c:116:[0-9]+: error: 
//match_regex: Test_name.c:118:[0-9]+: error: 
//match_regex: Test_name.c:119:[0-9]+: error: 
//match_regex: Test_name.c:120:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405, DECISIONS.md E169 (13), (14): -fno-contract-analysis-skip-dead-calls
// gives a call that folding proves is never evaluated its analysis
// evaluation at the call, as any other call: every dead call of
// p4405-analysis-dead-call.C is analyzed here, and g3's precondition is
// always false there, in templates and non-templates alike (but for the
// discarded statement of a template, which is never instantiated).  The
// nested walk of a predicate skips such an operand whatever the option
// says (`h7', `h8').
//
// Mirror: clang/test/Contracts/p4405-analysis-dead-call-no-skip.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O2" }
// { dg-additional-options "-fno-contract-analysis-skip-dead-calls" }
// { dg-additional-options "-fcontract-evaluation-semantic=ignore" }
// { dg-additional-options "-fcontract-analysis-semantic=prove" }

int g3 (int x) pre (x != 0);	// { dg-message "the predicate of the contract assertion" }
				// { dg-bogus "precondition of a call" "" { target *-*-* } .-1 }
int g3 (int x) { return 10 / x; }
int g4 (int x) pre (x != 0);
int g4 (int x) { return 10 / x; }

constexpr bool k = false;

// Never evaluated: analyzed all the same.
int h1 () { return 1 ? g4 (1) : g3 (0); }	// { dg-error "contract predicate is always false" }
int h1b () { return 0 ? g3 (0) : 2; }		// { dg-error "contract predicate is always false" }
bool h2 () { return false && g3 (0); }		// { dg-error "contract predicate is always false" }
bool h2b () { return true || g3 (0); }		// { dg-error "contract predicate is always false" }
bool h2c () { return 0 && g3 (0); }		// { dg-error "contract predicate is always false" }
bool h2d () { return 1 || g3 (0); }		// { dg-error "contract predicate is always false" }
int h3 (int y) { if (0) return g3 (0); return y; }	// { dg-error "contract predicate is always false" }
int h3b (int y) { if (1) return y; else return g3 (0); }	// { dg-error "contract predicate is always false" }
int h5 () { return k ? g3 (0) : 2; }		// { dg-error "contract predicate is always false" }
int h5b (int y) { if (k) return g3 (0); return y; }	// { dg-error "contract predicate is always false" }
int h6 () { if constexpr (false) return g3 (0); return 1; }	// { dg-error "contract predicate is always false" }
int h6b () { if constexpr (true) return 1; else return g3 (0); }	// { dg-error "contract predicate is always false" }
bool h9 () { return 1 || 0 && g3 (0); }		// { dg-error "contract predicate is always false" }
bool h9b () { return true || (false && g3 (0)); }	// { dg-error "contract predicate is always false" }
int h9c (int y) { return y ? (0 ? g3 (0) : 1) : 2; }	// { dg-error "contract predicate is always false" }

// Evaluated, or not known never to be: analyzed.
int live1 () { return 0 ? 2 : g3 (0); }		// { dg-error "contract predicate is always false" }
bool live2 () { return true && g3 (0); }	// { dg-error "contract predicate is always false" }
bool live3 () { return 0 || g3 (0); }		// { dg-error "contract predicate is always false" }
int live4 (int y) { if (0) return y; else return g3 (0); }	// { dg-error "contract predicate is always false" }
int live5 (int y) { if (1) return g3 (0); return y; }	// { dg-error "contract predicate is always false" }
int live6 (int y) { if (0) return y; return g3 (0); }	// { dg-error "contract predicate is always false" }
bool live7 () { return 0 && g3 (0) || g3 (0); }	// { dg-error "contract predicate is always false" }
int h4 (int y) { return y > 0 && y < 0 ? g3 (0) : y; }	// { dg-error "contract predicate is always false" }

// Predicates: the nested walk, unchanged.
int h7 (int y) { contract_assert ((1 ? g4 (1) : g3 (0)) == 10); return y; }	// { dg-bogus "(cannot be proven|always false)" }
int h8 (int y) { contract_assert (false && g3 (0)); return y; }	// { dg-error "contract predicate is always false" }

// Templates: a non-dependent condition, and a dependent one.  The
// discarded statement of a template's `if constexpr' is never instantiated
// (`t6', `tn6'), so it has no calls to analyze.
template <typename T> int t1 () { return 1 ? g4 (1) : g3 (0); }	// { dg-error "contract predicate is always false" }
template <typename T> int t1b () { return 0 ? g3 (0) : 2; }	// { dg-error "contract predicate is always false" }
template <typename T> bool t2 () { return false && g3 (0); }	// { dg-error "contract predicate is always false" }
template <typename T> bool t2b () { return true || g3 (0); }	// { dg-error "contract predicate is always false" }
template <typename T> bool t2c () { return 0 && g3 (0); }	// { dg-error "contract predicate is always false" }
template <typename T> int t3 (int y) { if (0) return g3 (0); return y; }	// { dg-error "contract predicate is always false" }
template <typename T> int t3b (int y) { if (1) return y; else return g3 (0); }	// { dg-error "contract predicate is always false" }
template <typename T> int t5 () { return k ? g3 (0) : 2; }	// { dg-error "contract predicate is always false" }
template <typename T> int t6 () { if constexpr (false) return g3 (0); return 1; }	// { dg-bogus "(cannot be proven|always false)" }
template <int N> int tn1 () { return N ? g4 (1) : g3 (0); }	// { dg-error "contract predicate is always false" }
template <int N> bool tn2 () { return N && g3 (0); }	// { dg-error "contract predicate is always false" }
template <int N> int tn3 (int y) { if (N) return g3 (0); return y; }	// { dg-error "contract predicate is always false" }
template <int N> int tn6 () { if constexpr (N) return g3 (0); return 1; }	// { dg-bogus "(cannot be proven|always false)" }
template <typename T> int tlive1 () { return 0 ? 2 : g3 (0); }	// { dg-error "contract predicate is always false" }
template <int N> bool tlive2 () { return N && g3 (0); }	// { dg-error "contract predicate is always false" }
template <typename T> int th4 (int y) { return y > 0 && y < 0 ? g3 (0) : y; }	// { dg-error "contract predicate is always false" }

template int t1<int> ();
template int t1b<int> ();
template bool t2<int> ();
template bool t2b<int> ();
template bool t2c<int> ();
template int t3<int> (int);
template int t3b<int> (int);
template int t5<int> ();
template int t6<int> ();
template int tn1<1> ();
template bool tn2<0> ();
template int tn3<0> (int);
template int tn6<0> ();
template int tlive1<int> ();
template bool tlive2<1> ();
template int th4<int> (int);
