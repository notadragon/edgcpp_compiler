//remark:contracts: violated predicates of member templates folding over a function parameter that expands the class's pack
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex:line 15: error: contract predicate is false in constant expression
//match_regex:line 17: error: contract predicate is false in constant expression
//match_regex:line 19: error: contract predicate is false in constant expression
//match_regex:line 22: error: contract predicate is false in constant expression
// Also runs under the C-generating back end.  The fold is instantiated for
// each instance of the member template (EDG-53), and each element of the
// pack is checked: a violation by the last one is found.  A violation is
// reported at its predicate, on the first declaration.

template<class... Ts> struct S {
  template<class U> constexpr int t(U u, Ts... p) const
    pre(((p < u) && ...)) { return 0; }
  template<class U> constexpr int r(const U u, const Ts... p) const
    post(x: ((p + x < u) && ...)) { return 2; }
  template<class U> constexpr int d(U u, Ts... p) const
    pre(((p < u) && ...));
  struct N {
    template<class U> constexpr int t(U u, Ts... p) const
      pre(((p < u) && ...)) { return 3; }
  };
};
template<class... Ts> template<class U>
constexpr int S<Ts...>::d(U u, Ts... p) const
  pre(((p < u) && ...)) { return 4; }

S<int, int> s;
S<int, int>::N sn;
static_assert(s.t(3, 1, 4) == 0);           // Error
static_assert(s.r(6, 1, 4) == 2);           // Error
static_assert(s.d(3, 1, 4) == 4);           // Error
static_assert(sn.t(3, 1, 4) == 3);          // Error
