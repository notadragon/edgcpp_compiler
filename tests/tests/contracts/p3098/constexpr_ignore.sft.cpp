//remark:contracts: P3098 in constant evaluation under ignore: the captures are not initialized, and the predicate is not evaluated
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options_all:--contracts_p3098 --contract_evaluation_semantic=ignore
struct D { int v; constexpr D(int v) : v(v) {} constexpr ~D() {} };
constexpr int bad(int x) post [y = x] (r: r == y) { return x + 1; }
constexpr int dt(const int x) post [d = D(x)] (d.v != x) { return x; }
static_assert(bad(1) == 2);
static_assert(dt(1) == 1);
int x;
