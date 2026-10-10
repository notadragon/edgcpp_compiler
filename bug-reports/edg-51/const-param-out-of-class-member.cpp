// The out-of-class definition's parameter x is not const: top-level
// cv-qualifiers are not part of the function type ([dcl.fct]/5), and the
// definition declares x without const.  The assignment is well-formed.
template<class T> struct C { void m(const T x); };
template<class T> void C<T>::m(T x) { x = 1; }
template struct C<int>;
