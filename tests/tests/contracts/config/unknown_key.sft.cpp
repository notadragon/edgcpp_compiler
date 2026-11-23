//remark:contracts: an unknown key in a configuration is a warning (tag contract_configuration), and its entry is skipped
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce '--contract_configuration=[{"output":{"semantic":"ignore","extra":1}}]'
//match_regex:Command-line warning: contract configuration \(<command-line>:1:\d+\): unknown key "extra" in "output" object; the entry is skipped
//match_regex:line 7: error: contract predicate is false in constant expression
// The skipped entry would have made the assertion ignored.
constexpr int f(int x) pre(x > 0) { return x; }
constexpr int v = f(0);
int use() { return v; }
