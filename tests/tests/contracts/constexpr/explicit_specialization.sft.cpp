//remark:contracts: an explicit specialization evaluates its own contract assertions in constant evaluation
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex:line 17: error: contract predicate is false in constant expression
//match_regex:line 24: error: contract predicate is false in constant expression
//match_regex:line 34: error: contract predicate is false in constant expression
//match_regex:line 38: error: contract predicate is false in constant expression
// An explicit specialization has its own function contract specifiers, not
// its template's: of a function template, of a member of a class template,
// and of a member template (whose instances are declared from the member
// template's declaration, but use the names the specialization gives the
// parameters).  Each template's precondition fails for every call below, so
// only the specializations' are evaluated.  A specialization without
// contract specifiers has none.

template <class T> constexpr int g(T t) pre(t > 100) { return 0; }
template <> constexpr int g<double>(double a) pre(a > 1) { return 1; }
static_assert(g(0.5) == 1);                 // Error
static_assert(g(5.0) == 1);                 // OK

template <class T> struct B {
  constexpr int m(T t) const pre(t > 100) { return 0; }
};
template <> constexpr int B<double>::m(double a) const pre(a > 1) { return 1; }
static_assert(B<double>().m(0.5) == 1);     // Error
static_assert(B<double>().m(5.0) == 1);     // OK

template <class T> struct A {
  template <class P> constexpr int f(T t, P) const pre(t > 100) { return 0; }
  template <class... P> constexpr int v(T t, P...) const pre(t > 100)
    { return 0; }
};
template <> template <class Q>
constexpr int A<double>::f(double a, Q) const pre(a > sizeof(Q)) { return 1; }
static_assert(A<double>().f(0.5, 'c') == 1);  // Error
static_assert(A<double>().f(5.0, 'c') == 1);  // OK
template <> template <class... Q>
constexpr int A<double>::v(double a, Q... q) const pre(sizeof...(q) == 2)
  { return 1; }
static_assert(A<double>().v(5.0, 1) == 1);     // Error
static_assert(A<double>().v(5.0, 1, 2) == 1);  // OK

// Declared before it is defined, with the parameter renamed.
template <> constexpr int g<long>(long a) pre(a > 1);
template <> constexpr int g<long>(long b) pre(b > 1) { return 2; }
static_assert(g(5L) == 2);                  // OK

// No contract specifiers.
template <> constexpr int g<int>(int a) { return 3; }
template <> constexpr int B<int>::m(int a) const { return 3; }
template <> template <class Q>
constexpr int A<int>::f(int a, Q) const { return 3; }
static_assert(g(1) + B<int>().m(1) + A<int>().f(1, 1) == 9);  // OK
