//remark:contracts: contract specifiers declared in a function's scope, or instantiated there
//type:rp
//options_all:--contract_evaluation_semantic=quick_enforce
// The predicates of these specifiers are scanned while a function's memory
// region is current, while the specifiers belong to file-scope routines; a
// Debug build checks that file-scope IL never points into a function's
// region (EDG-46).  Runs with both back ends; no assertion is violated.
auto ns_lambda = [](const int b) -> int pre(b > 0) post(r: r == b) {
  return b;
};
template <class T> T inst(const T x) pre(x > 0) post(r: r == x) { return x; }
int f(int lo) {
  // A block-scope declaration.
  int blk(const int x) pre(x > 0) post(r: r == x);
  struct L {
    int m(const int x) const pre(x > 0) post(r: r == x) { return x; }
  };
  auto own = [lo](const int b) -> int pre(b > lo) post(r: r == b) { return b; };
  return blk(lo) + L().m(lo) + own(lo + 1) + ns_lambda(lo) + inst(lo);
}
int blk(const int x) pre(x > 0) post(r: r == x) { return x; }
int main() { return f(1) == 6 ? 0 : 1; }
