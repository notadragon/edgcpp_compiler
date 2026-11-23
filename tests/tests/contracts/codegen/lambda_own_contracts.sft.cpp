//remark:contracts: a lambda's own preconditions and postconditions name its captures
//type:rp
//options_all:--contract_evaluation_semantic=quick_enforce
// P2900 [expr.prim.lambda.capture]: the predicates of a lambda's function
// contract specifiers are in the scope of its call operator, so they name
// its captures, explicit, implicit or init-captures.  Runs with both back
// ends; no assertion is violated.
int f() {
  int lo = 1;
  auto l1 = [lo](int b) pre(b > lo) { return b; };
  auto l2 = [&](int b) pre(b > lo) { return b + lo; };
  auto l3 = [c = lo](int b) pre(b > c) { return b; };
  auto l4 = [lo](int b) -> int post(r: r > lo) { return b; };
  auto l5 = [=](const int b) -> int pre(b > lo) post(r: r == b + lo) { return b + lo; };
  return l1(2) + l2(2) + l3(2) + l4(2) + l5(2);
}
constexpr int k() {
  int lo = 1;
  auto l = [lo](int b) -> int pre(b > lo) post(r: r > lo) { return b; };
  return l(2);
}
static_assert(k() == 2);
int main() { return f() == 12 ? 0 : 1; }
