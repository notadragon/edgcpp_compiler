//remark:contracts: a parameter named only in a contract predicate is used: no "set but never used" warning when the body assigns to it (EDG-74)
//type:fp
//options_all:--contract_evaluation_semantic=ignore
int f(int i) pre (i > 0) { i = 2; return 0; }
int g(const int i, int j) post (r: r == i) pre (j > 0) {
  j = 0; return i; }
struct S {
  int m(int k) pre (k != 0) { k = 1; return 0; }
};
template<class T> T t(T v) pre (v > 0) { v = 0; return T(); }
int u() { return t(1); }
