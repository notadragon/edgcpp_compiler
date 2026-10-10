//remark:contracts: a parameter a postcondition odr-uses must be const on a function template's redeclarations, in each instance
//type:fn
//match_regex:line 8: error: value parameter "a" used in a postcondition must be declared const
//match_regex:line 21: error: value parameter "b" used in a postcondition must be declared const
// A redeclaration without the postcondition, whose parameter type is
// dependent, is checked in each instance (once per declaration).
template <typename T> void f(T const a) post(a);
template <typename T> void f(T a);              // Error in f<int>
template <typename T> void f(T const a) {}
template void f<int>(int);
template void f<long>(long);
template <typename T> void g(T const a) post(a);
template <typename T> void g(T a);              // OK, a reference in g<int &>
template <typename T> void g(T const a) {}
template void g<int &>(int &);
template <typename T> void h(T const a) post(a);
template <typename T> void h(T a);              // OK, const in h<const int>
template <typename T> void h(T const a) {}
template void h<const int>(const int);
template <typename T> int k(T const a) post(a > 0);
template <typename U> int k(U b);               // Error in k<int>
template <typename T> int k(T const a) { return a; }
int use() { return k(1); }
// A friend redeclaring a member of a class template is not a function
// template's redeclaration (it once crashed the check).
template <typename T> struct A { int m(int); int n(const int a) post(a > 0); };
class C {
  template <typename T> friend int A<T>::m(int);
  template <typename T> friend int A<T>::n(const int a);
};
