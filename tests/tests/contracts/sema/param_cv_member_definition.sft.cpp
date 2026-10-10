//remark:a parameter variable takes its top-level cv-qualifiers from the definition, also for a member of a class template defined outside the class
//type:fn
//match_regex:line 19: error: expression must be a modifiable lvalue
//match_regex:line 25: error: expression must be a modifiable lvalue
//match_regex:line 43: error: expression must be a modifiable lvalue
//match_regex:line 46: error: expression must be a modifiable lvalue
//match_regex:line 58: error: expression must be a modifiable lvalue
//match_regex:line 79: error: value parameter "x" used in a postcondition must be declared const
//require:BACK_END_IS_CP_GEN_BE 1
// Not contracts-specific (an upstream fix, EDG-51): top-level cv-qualifiers
// of a parameter are not part of the function type ([dcl.fct]/5), so in a
// definition they come from the definition only.  A member of a class
// template instance is declared by rescanning the in-class declaration; its
// parameters had kept that declaration's const.  Const from a template
// argument stays.
template <class T> struct A {
  void m1(const T x);
  void m2(T x);
  void m3(const T x) { x = 1; }                 // Error, in A<int>
  template <class U> void m4(const T x, U);
  void m5(const T x);
};
template <class T> void A<T>::m1(T x) { x = 1; }  // OK
template <class T> void A<T>::m2(const T x) {
  x = 1;                                         // Error, in A<int>
}
template <class T> template <class U>
void A<T>::m4(T x, U) { x = 1; }                // OK
template <> void A<long>::m5(long x) { x = 1; }  // OK
template <class T> void A<T>::m5(T x) { x = 1; }  // OK
template struct A<int>;
template void A<int>::m4(int, char);
template <> struct A<const int> {
  void n(const int x);
};
void A<const int>::n(int x) { x = 1; }         // OK, not a template member

template <class T> struct B {
  void m1(T x);
  void m2(const T x);
};
template <class T> void B<T>::m1(T x) {
  x = 1;                                         // Error, in B<const int>
}
template <class T> void B<T>::m2(T x) {
  x = 1;                                         // Error, in B<const int>
}
template struct B<const int>;

struct S {
  template <class U> void m(const U x);
  template <class U> void m2(U x);
  template <class U> friend void f(const U x);
  void n(const int x);
};
template <class U> void S::m(U x) { x = 1; }    // OK
template <class U> void S::m2(const U x) {
  x = 1;                                         // Error, in S::m2<int>
}
template <class U> void f(U x) { x = 1; }       // OK
void S::n(int x) { x = 1; }                     // OK
template void S::m(int);
template void S::m2(int);
template void f(int);

template <class T> struct C {
  template <class U> void m(const U x);
};
template <class T> template <class U>
void C<T>::m(U x) { x = 1; }                    // OK
template void C<int>::m(int);

// [dcl.contract.func]: a parameter a postcondition odr-uses must be const in
// every declaration, so also in the definition (EDG-27).
template <class T> struct D {
  void m(const T x) post(x > 0);
  void n(const T x) post(x > 0);
};
template <class T> void D<T>::m(T x) {}         // Error, in D<int>
template <class T> void D<T>::n(const T x) {}   // OK
template struct D<int>;
