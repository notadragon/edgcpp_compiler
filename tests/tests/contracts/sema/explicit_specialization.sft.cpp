//remark:contracts: function contract specifiers of explicit specializations and their redeclarations
//type:fn
//match_regex:line 21: error: mismatched contract condition in declaration
//match_regex:line 23: error: declaration adds contracts to
//match_regex:line 25: error: mismatched contract condition in declaration
//match_regex:line 27: error: declaration adds contracts to
//match_regex:line 30: error: mismatched contract condition in declaration
//match_regex:line 33: error: declaration adds contracts to
//match_regex:line 41: error: identifier "zz" is undefined
// The first declaration of an explicit specialization provides its
// contracts (whatever its template's are); a redeclaration of the
// specialization repeats them exactly or omits them, as for any function:
// of a function template, of a member of a class template, and of a member
// template.  The predicate of a specialization declared by an unqualified
// template-id is scanned too.
template <class T> void ft(T x) pre(x > 0);
template <class T> struct B { void m(T x) pre(x > 0); };
template <class T> struct A { template <class P> void f(T x, P) pre(x > 0); };

template <> void ft<int>(int x) pre(x > 5);
template <> void ft<int>(int x) pre(x > 6) {}            // Error, condition
template <> void ft<long>(long x);
template <> void ft<long>(long y) pre(y > 0) {}          // Error, adds
template <> void B<int>::m(int x) pre(x > 5);
template <> void B<int>::m(int x) pre(x > 6) {}          // Error, condition
template <> void B<long>::m(long x);
template <> void B<long>::m(long x) pre(x > 0) {}        // Error, adds
template <> template <class Q> void A<int>::f(int x, Q) pre(x > 4);
template <> template <class Q>
void A<int>::f(int x, Q) pre(x > 5) {}                   // Error, condition
template <> template <class Q> void A<long>::f(long x, Q);
template <> template <class Q>
void A<long>::f(long x, Q) pre(x > 0) {}                 // Error, adds

template <> void ft<char>(char a) pre(a > 1);
template <> void ft<char>(char b) pre(b > 1) {}          // OK, renamed
template <> template <class Q> void A<char>::f(char a, Q q) pre(a > q);
template <> template <class R>
void A<char>::f(char b, R r) pre(b > r) {}               // OK, renamed
template <> void ft<short>(short a) {}                   // OK, none
template <> void ft<float>(float a) pre(zz > a);         // Error
