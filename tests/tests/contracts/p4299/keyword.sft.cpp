//remark:contracts: with --contracts_p4299 _Pre, _Post and _ContractAssert are keywords (as in Clang), so they cannot name a variable; a macro named _Pre is expanded as a macro, and a _Pre from a macro's expansion is the keyword
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options_all:--contracts_p4299
//match_regex:line 9: error: expected an identifier
//match_regex:line 10: error: expected an identifier
//match_regex:line 11: error: expected an identifier
//match_regex:line 12: error: expected an expression
int _ContractAssert = 1;   // Error
int _Pre = 1;              // Error
int _Post = 1;             // Error
int g() { return _Pre; }   // Error
#define _Pre post
int f(int x) _Pre(r : r > 0) { return x; }
#undef _Pre
#define PRECONDITION(c) _Pre(c)
int h(int x) PRECONDITION(x > 0) { return x; }
