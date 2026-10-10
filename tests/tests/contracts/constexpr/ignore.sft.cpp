//remark:contracts: nothing is evaluated in constant evaluation under ignore
//type:fp
//options_all:--contract_evaluation_semantic=ignore
// Also runs under the C-generating back end.
constexpr int f(int x) pre(x > 0) post(r: r > 0) {
  contract_assert(x != 0);
  return x;
}
static_assert(f(0) == 0);
int not_constexpr(int x) { return x; }
constexpr int g(int x) pre(not_constexpr(x) > 0) { return x; }
static_assert(g(1) == 1);
