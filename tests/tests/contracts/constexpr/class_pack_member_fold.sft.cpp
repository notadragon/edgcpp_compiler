//remark:contracts: constant evaluation checks a fold over a class template's pack in a member declared in the class and defined outside it (EDG-124)
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex:line 13: error: contract predicate is false in constant expression
//match_regex:line 14: error: contract predicate is false in constant expression
// Also runs under the C-generating back end.  The specifiers are the
// in-class declaration's, instantiated with the class's template declaration
// information (templates/class_pack_member_fold has the run-time checks).
// The holding calls are constant; each violation is reported at its
// predicate, on the in-class declaration.

template <class... T> struct Z {
  constexpr int f(const T... t) const pre(((t > 0) && ...));
  constexpr auto g(const T... t) const post(r: ((t > r) && ...));
};
template <class... T>
constexpr int Z<T...>::f(const T... b) const { return 0; }
template <class... T>
constexpr auto Z<T...>::g(const T... t) const post(r: ((t > r) && ...)) {
  return 0;
}

constexpr Z<int, int> z;
static_assert(z.f(1, 2) == 0);              // OK
static_assert(z.g(1, 2) == 0);              // OK
constexpr int bad_f = z.f(1, -2);           // Error
constexpr int bad_g = z.g(1, 0);            // Error
