//remark: imported from gcc:p3098-unexpanded-pack-capture.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098 --contract_evaluation_semantic=enforce
//match_regex: ", line 34: (?:catastrophic )?error
//match_regex: ", line 36: (?:catastrophic )?error
//match_regex: ", line 38: (?:catastrophic )?error
//match_regex: ", line 44: (?:catastrophic )?error
//match_regex: ", line 46: (?:catastrophic )?error
//match_regex: ", line 48: (?:catastrophic )?error
//match_regex: ", line 50: (?:catastrophic )?error
//match_regex: ", line 54: (?:catastrophic )?error
//match_regex: ", line 56: (?:catastrophic )?error
//match_regex: ", line 58: (?:catastrophic )?error
//match_regex: ", line 60: (?:catastrophic )?error
// EDG: adapted -- bad, bad2 and d6 get a second error from EDG's parser
// recovery, "pack expansion does not make use of any argument packs".
// A P3098 postcondition capture that is not a pack may not name a parameter
// pack unexpanded, as a lambda capture may not: `[xs]' wants `[...xs]', and
// `[y = xs]' wants `[...y = xs]' (GCC-638).  An init-capture pack named
// unexpanded in the predicate is an unexpanded pack like any other
// (GCC-639).  Neither was diagnosed in the template, and the instantiation
// ICEd or crashed.  Only an init-capture puts the ellipsis first: `[...xs]'
// is ill-formed, as in a lambda, and was accepted as `[xs...]' (GCC-680);
// `[y... = xs]' is ill-formed too, and is diagnosed as such rather than with
// generic parse errors (GCC-681).
//
// Mirrors: clang/test/Contracts/p3098-unexpanded-pack-capture.cpp and
// p3098-unexpanded-pack-predicate.cpp in the llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098 -fcontract-evaluation-semantic=enforce" }

template <class... T>
int p3 (T... xs) post [xs] (true) { return 0; }		// { dg-error "not expanded" }
template <class... T>
int p4 (T... xs) post [y = xs] (true) { return 0; }		// { dg-error "not expanded" }
template <class... T>
int p5 (T... xs) post [...y = xs] (y > 0) { return 0; }	// { dg-error "not expanded" }
template <class... T>
int ok (T... xs) post [...y = xs] (((y > 0) && ...)) { return 0; }	// OK
template <class... T>
int ok2 (T... xs) post [xs...] (((xs > 0) && ...)) { return 0; }	// OK
template <class... T>
int bad (T... xs) post [...xs] (((xs > 0) && ...)) { return 0; }	// { dg-error "no initializer is written .xs\\.\\.\\.., not .\\.\\.\\.xs." }
template <class... T>
int bad2 (T... xs) post [...xs...] (((xs > 0) && ...)) { return 0; }	// { dg-error "no initializer" }
template <class... T>
int bad3 (T... xs) post [y... = xs] (((y > 0) && ...)) { return 0; }	// { dg-error "init-capture is written .\\.\\.\\.y =., not .y\\.\\.\\. =." }
template <class... T>
int bad4 (T... xs) post [...y... = xs] (((y > 0) && ...)) { return 0; }	// { dg-error "init-capture is written" }

// The same on a declaration, whose contracts are parsed with it.
template <class... T>
int d3 (T... xs) post [xs] (true);			// { dg-error "not expanded" }
template <class... T>
int d5 (T... xs) post [...y = xs] (y > 0);		// { dg-error "not expanded" }
template <class... T>
int d6 (T... xs) post [...xs] (((xs > 0) && ...));	// { dg-error "no initializer" }
template <class... T>
int d7 (T... xs) post [y... = xs, z = 0] (((y > z) && ...));	// { dg-error "init-capture is written" }

int main () { return ok (1, 2) + ok2 (1, 2); }
