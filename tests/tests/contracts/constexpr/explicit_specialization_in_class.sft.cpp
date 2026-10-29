//remark:contracts: an explicit specialization declared in a class evaluates its own contract assertions, naming later members, in constant evaluation
//require:DO_IL_LOWERING 1
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex:line 17: error: contract predicate is false in constant expression
//match_regex:line 19: error: contract predicate is false in constant expression
//match_regex:line 31: error: contract predicate is false in constant expression
// The predicate of an explicit specialization declared in a class (CWG727)
// names a member declared after it ([class.mem.general]), in a class and in
// a class template; the member template's precondition fails for every call
// below, so only the specializations' are evaluated.  (Only under the
// edg_x86_64 configuration: in GNU mode, as in g++, an explicit
// specialization cannot be declared in a class.)

struct S {
  template <class T> constexpr int f(T a) const pre(a > 100) { return 0; }
  template <> constexpr int f<int>(int a) const pre(a > lim()) { return 1; }
  template <class T> constexpr int g(T a) const post(r: r > 100);
  template <> constexpr int g<int>(int a) const post(r: r > lim());
  template <> constexpr int g<int>(int a) const post(s: s > lim())
    { return a; }
  static constexpr int lim() { return 5; }
};
static_assert(S().f(7) == 1);               // OK
static_assert(S().f(3) == 1);               // Error
static_assert(S().g(7) == 7);               // OK
static_assert(S().g(3) == 3);               // Error

template <class U> struct C {
  template <class T> constexpr int f(T a) const pre(a > 100) { return 0; }
  template <> constexpr int f<int>(int a) const pre(a > lim()) { return 1; }
  template <> constexpr int f<char>(char a) const { return 2; }
  static constexpr U lim() { return 5; }
};
static_assert(C<int>().f(7) == 1);          // OK
static_assert(C<int>().f('a') == 2);        // OK
static_assert(C<long>().f(7) == 1);         // OK
static_assert(C<long>().f(3) == 1);         // Error
