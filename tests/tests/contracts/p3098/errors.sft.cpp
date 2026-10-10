//remark:contracts: P3098: what a capture list cannot hold, and where it cannot appear
//type:fn
//options_all:--contracts_p3098
//match_regex:line 19: error: "this" cannot be captured in a postcondition
//match_regex:line 20: error: a capture default is not allowed in a postcondition
//match_regex:line 21: error: capture by reference is not allowed in a postcondition
//match_regex:line 22: error: only a function parameter can be captured without an initializer in a postcondition
//match_regex:line 23: error: "x" is captured more than once in the postcondition
//match_regex:line 24: error: a capture list is allowed only on a postcondition
//match_regex:line 26: error: a capture list is allowed only on a postcondition
//match_regex:line 27: error: pack expansion does not make use of any argument packs
//match_regex:line 28: error: pack expansion does not make use of any argument packs
//match_regex:line 29: error: parameter pack "xs" was referenced but not expanded
//match_regex:line 30: error: pack expansion does not make use of any argument packs
//match_regex:line 31: error: parameter pack "y" was referenced but not expanded
//match_regex:line 32: error: "y" is captured more than once in the postcondition
int g;
struct S {
  void a(int x) post [this] (true);
  void b(int x) post [=] (true);
  void c(int x) post [&x] (true);
  void d(int x) post [g] (true);
  void e(int x) post [x, x = 1] (true);
  void f(int x) pre [x] (true);
};
void h(int x) { contract_assert [x] (true); }
void p1(int i) post [i...] (true);
void p2(int i) post [...y = i] (true);
template<class... T> void p3(T... xs) post [xs] (true);
template<class... T> void p4(int n, T... xs) post [...y = n] (true);
template<class... T> void p5(T... xs) post [...y = xs] (y > 0);
template<class... T> void p6(T... xs) post [...y = xs, ...y = xs] (true);
