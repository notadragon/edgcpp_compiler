//remark:contracts: a potentially-throwing predicate under quick_enforce draws no "may be used uninitialized" warning from g++
//require:BACK_END_IS_CP_GEN_BE 1
//type:cp
//options_all:--contract_evaluation_semantic=quick_enforce -O2 -Wall -Wextra
// Mirror of our GCC's contract-quick-enforce-throwing-predicate-uninit.C
// (GCC-689, EDG-122): GCC's own lowering left the check's result temporary
// unset after the quick_enforce trap call in its catch (...) handler, so
// g++ warned at -O2.  Through edg-gxx g++ checks the assertions cpfe-cp
// puts out as written, with the fixed lowering: the recording has no
// warning.
bool positive(int);

void pre_fn(int x) pre(positive(x)) {}                     // OK, no warning

int post_fn(int x) post(r: positive(r)) { return x; }      // OK, no warning

void assert_fn(int x) { contract_assert(positive(x)); }    // OK, no warning

int main() {
  pre_fn(1);
  assert_fn(2);
  return post_fn(3);
}
