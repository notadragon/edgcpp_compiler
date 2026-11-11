//remark:contracts: attributes on contract assertions and result names
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contract_evaluation_semantic=enforce
//match_regex:line 11: warning: attribute namespace "foo" is unrecognized
//match_regex:line 12: warning: attribute "maybe_unused" does not apply here
// The grammar allows an attribute-specifier-seq after pre, post and
// contract_assert (it appertains to the assertion, to which no attribute
// applies), and on a result name (appertaining to the result binding, where
// maybe_unused is valid).
int f(int x) pre [[foo::bar]] (x > 0) post [[foo::bar]] (r: r > 0) {
  contract_assert [[maybe_unused]] (x > 0);
  return x;
}
int g(int x) post(r [[maybe_unused]] : r > 0) { return x; }
int h(int x) post(r [[maybe_unused]] [[foo::bar]] : r > 0) { return x; }
int main() { return f(1) + g(1) + h(1) == 3 ? 0 : 1; }
