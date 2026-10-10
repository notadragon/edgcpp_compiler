//remark:contracts: a parameter declarator has no function contract specifiers
//type:fn
//match_regex:line 7: error: a function contract specifier can appear only on the declarator of a function declaration
//match_regex:line 8: error: a function contract specifier can appear only on the declarator of a function declaration
// A parameter-declaration has no function-contract-specifier-seq, even when
// the parameter has function type.
void takes_fn(int bar() pre(true));
void defines_fn(int bar() pre(false)) { (void)bar; }
void ok(int bar()) pre(true);
