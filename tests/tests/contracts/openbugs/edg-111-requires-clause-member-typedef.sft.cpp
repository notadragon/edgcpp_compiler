//remark:contracts: EDG-111 (stock): a requires-clause on a member typedef of function type in a class template is accepted
//type:fp
// The recording is the current, wrong behavior (see bug-reports/edg-111/):
// when this test deviates, the bug may be fixed.  Not contracts-specific.
// [dcl.decl.general]: a requires-clause is allowed only on a declarator
// that declares a templated function; the same typedef at namespace scope,
// and a non-template one, are diagnosed.  From GCC's
// g++.dg/cpp2a/concepts-requires-declarator.C (EDG-101).
template <class T> concept C = sizeof(T) > 0;
template <class T> struct A {
  typedef int F(int) requires C<T>;   // Error, not given
};
A<int> a;
