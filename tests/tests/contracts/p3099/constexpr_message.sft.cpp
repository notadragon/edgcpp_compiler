//remark:contracts: P3099: a violation in constant evaluation is reported with the diagnostic message
//type:fn
//options_all:--contracts_p3099 --contract_evaluation_semantic=quick_enforce
//match_regex:line 9: error: contract predicate is false in constant expression \(x must be positive\)
//match_regex:line 11: error: contract predicate is false in constant expression \(r: "quoted" \\back\)
//match_regex:line 13: error: contract predicate is false in constant expression \(\)
//match_regex:line 14: error: contract predicate is false in constant expression$
// An empty message is a message; no message gives the plain diagnostic.
constexpr int f(int x) pre(x > 0, "x must be positive") { return x; }
constexpr int g(int x)
  post(r: r > 0, "r: \"quoted\" \\back") { return x; }
constexpr int h(int x) {
  contract_assert(x > 0, "");
  contract_assert(x > -1);
  return x;
}
constexpr int v1 = f(0);
constexpr int v2 = g(0);
constexpr int v3 = h(-1);
int use() { return v1 + v2 + v3; }
