//remark:contracts: in constant evaluation assume is ignore: neither a false nor a non-constant predicate is diagnosed
//type:fp
//options_sep:!
//options:--contracts_allow_assume --contract_evaluation_semantic=assume!--contract_evaluation_semantic=assume!--contracts_allow_assume '--contract_configuration=[{"match":{"constexpr":true},"output":{"semantic":"assume"}},{"output":{"semantic":"ignore"}}]'
// (No system headers: also runs under the C-generating back end.)
int g;
constexpr int f(int x) pre (x > 0) { contract_assert(x > 1); return x; }
constexpr int h(int x) pre (g == 0) { return x; }
constexpr int v = f(0);
constexpr int w = h(1);
