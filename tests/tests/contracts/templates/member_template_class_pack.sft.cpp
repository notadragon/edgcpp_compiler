//remark:contracts: the predicate of a member template of a class template is instantiated for each instance, not with the class
//type:fp
//options_all:--contract_evaluation_semantic=quick_enforce
// Also runs under the C-generating back end.  Instantiating a class does not
// instantiate the contract assertions of its member templates (P2900
// [temp.inst]); each instance of a member template has them instantiated
// from the template's.  A fold over a function parameter that expands the
// class's pack is then a fold over a pack (EDG-53), and a predicate that is
// invalid for the arguments of the class is not diagnosed until it is used.
// No predicate here is violated (constexpr/member_template_class_pack_violated
// has the violations).

template<class... Ts> struct S {
  template<class U> constexpr int t(U u, Ts... p) const
    pre(((p < u) && ...)) { return 0; }
  template<class U> constexpr int n(U u, Ts... p) const
    pre(sizeof...(p) == sizeof...(Ts))
    pre(sizeof(decltype(((p < u) && ...))) > 0) { return 1; }
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

S<int, int> s;                              // OK
S<int, int>::N sn;                          // OK
S<> e;                                      // OK
static_assert(s.t(5, 1, 4) == 0);           // OK
static_assert(s.n(5, 1, 4) == 1);           // OK
static_assert(s.r(9, 1, 4) == 2);           // OK
static_assert(s.d(5, 1, 4) == 4);           // OK
static_assert(sn.t(5, 1, 4) == 3);          // OK
static_assert(e.t(5) == 0);                 // OK
int use(int k) { return s.t(k, 1, 4) + sn.t(k, 1, 4); }

template<class T> struct C {
  template<class U> void m(U) pre(T::value) {}
};
C<int> c;                                   // OK, C<int>::m not used
