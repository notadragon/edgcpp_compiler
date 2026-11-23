//remark:contracts: in the C-generating configuration an entry for constant evaluation only may give any semantic, and assume is replaced by ignore
//require:DO_IL_LOWERING 1
//type:fp
//options_all:'--contract_configuration=[{"match":{"constexpr":true},"output":{"semantic":"observe"}},{"output":{"semantic":"assume"}}]'
//match_regex:line 9: warning: contract predicate is false in constant expression
// The violation is observed in constant evaluation (a warning), and the
// assertion is not checked at run time (assume: ignore).
extern "C" int puts(const char *);
constexpr int f(int x) pre(x > 0) { return x; }
constexpr int v = f(0);
int main() { puts(v == 0 ? "ok" : "bad"); return 0; }
