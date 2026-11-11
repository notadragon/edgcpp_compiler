//remark:contracts: the parameters a postcondition uses must be const on a definition whose body is a function-try-block too (EDG-83)
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex:line 6: error: value parameter "x" used in a postcondition must be declared const
// (No system headers: also runs under the C-generating back end.)
int h(int x) post (r: r > x) try { return x + 1; } catch (...) { return 0; }
