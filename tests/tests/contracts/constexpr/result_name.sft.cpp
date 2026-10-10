//remark:contracts: a postcondition's result name in constant evaluation
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options_all:--contract_evaluation_semantic=enforce
//match_regex:line 30: error: contract predicate is false in constant expression
// The result name designates the result object -- the object returned by
// reference for a reference return type, and the subobject a call
// initializes in place.
struct P { int x, y; };
constexpr P make(const int a) post(r: r.x == a && &r.y == &r.x + 1) { return {a, a}; }
static_assert(make(2).y == 2);
struct W { int z; P p = make(3); };
constexpr W w{};
static_assert(w.p.y == 3);

constexpr int ci = 4;
constexpr const int &ref(const int &a) post(r: &r == &a && r == 4) { return a; }
static_assert(ref(ci) == 4);
constexpr int by_value(const int &a) post(r: r == a) { return a; }
static_assert(by_value(4) == 4);
// A computed scalar result, used as a value and as an lvalue.
constexpr int inc(const int x) post(r: r == x + 1) { return x + 1; }
static_assert(inc(4) == 5);
constexpr bool positive(const int &v) { return v > 0; }
constexpr int inc2(const int x) post(r: positive(r)) { return x + 1; }
static_assert(inc2(4) == 5);
struct A { int z; int m = inc(4); };
constexpr A aa{};
static_assert(aa.m == 5);
constexpr P wrong(const int a) post(r: r.x != a) { return {a, a}; }
constexpr P pw = wrong(1);      // Error, at the "post"
