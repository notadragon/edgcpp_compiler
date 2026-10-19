//remark:contracts: constant evaluation of a postcondition whose lambda uses the result name
//type:fp
//options_all:--contract_evaluation_semantic=quick_enforce
// The result name designates the result also in the body of a lambda that
// the predicate calls.
constexpr int e() post(r: [&] { return r; }() == 5) { return 5; }
constexpr int g() post(r: [=] { return r + 1; }() == 6) { return 5; }
static_assert(e() == 5);
static_assert(g() == 5);
