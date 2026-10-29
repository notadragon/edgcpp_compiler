//remark:contracts: the contract assertions of templates are scanned in the template and instantiated on odr-use
//options:-DCASE=1;fn:-DCASE=2;fn:-DCASE=3;fp:-DCASE=4;fp:-DCASE=5;fp
// A predicate is scanned in its template, and instantiated for an instance
// when the instance is odr-used or its definition is instantiated -- not
// when it is named in an unevaluated operand.  A requires-expression in a
// predicate is substituted in an instantiation, so an invalid requirement
// makes it false rather than ill-formed.
#if CASE == 1
template <class T> struct C {
  void f(T x) pre(undeclared > 0);  // Error, in the template
};
#elif CASE == 2
template <class T> int f(T t) pre(t.m > 0);  // Error, f<int> is odr-used
int use() { return f(0); }
#elif CASE == 3
template <class T> constexpr int f() pre(T::error) { return 0; }
int use() { return sizeof(f<long>()); }  // OK, unevaluated
#elif CASE == 4
template <class T> bool f(T x) pre(!requires { ++x; }) { return true; }
template <class T> struct C {
  bool g(T x) pre(!requires { ++x; }) { return true; }
  template <class U> bool h(U u) pre(!requires { ++u; }) { return true; }
};
struct D { template <class U> bool k(U u) pre(!requires { ++u; }) { return true; } };
bool use() {
  auto l = [](auto y) pre(!requires { ++y; }) { return true; };
  return f<const int>(1) && C<const int>().g(1) && C<int>().h<const int>(1) &&
         D().k<const int>(1) && l.operator()<const int>(1);
}
#elif CASE == 5
struct S {
  template <class U> bool f(U u) pre(u < limit()) { return true; }
  static constexpr int limit() { return 10; }  // OK, declared after
};
bool use() { return S().f(1); }
#endif
