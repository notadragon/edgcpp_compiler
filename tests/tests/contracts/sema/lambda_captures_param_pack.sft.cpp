//remark:contracts: a lambda in a function template's precondition or postcondition captures the function parameter pack, implicitly or as "xs..." (EDG-106)
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DBAD=0;rp:-DBAD=1;rn:-DBAD=2;rn:-DBAD=3;rn
// (No system headers: also runs under the C-generating back end.)  Each
// lambda captures the elements of the pack, as the same lambda in a
// contract_assert in the body does.  From GCC's
// predicate-lambda-pack-capture.C (EDG-93).
template <class... T>
int g(T... xs) pre([=] { return ((xs > 0) && ...); }()) { return 0; }
template <class... T>
int p(const T... xs) post([&] { return ((xs > 0) && ...); }()) { return 0; }
template <class... T>
int h(T... xs) pre([xs...] { return ((xs > 0) && ...); }()) { return 0; }
template <class... T>
int s(T... xs) pre([&] { return sizeof...(xs) == 2; }()) { return 0; }
int main() {
  int r = g(1, 2) + p(1, 2) + h(1, 2) + s(1, 2) + g();
  if (BAD == 1) r += g(1, -2);
  if (BAD == 2) r += h(-1, 2);
  if (BAD == 3) r += p(1, -2);
  return r;
}
