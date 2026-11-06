//remark:contracts: postcondition parameters must be const on every declaration
//type:fn
//match_regex:line 20: error: value parameter "a" used in a postcondition must be declared const
//match_regex:line 23: error: an unnamed value parameter used in a postcondition must be declared const
//match_regex:line 24: error: value parameter "c" used in a postcondition must be declared const
//match_regex:line 25: error: value parameter "d" used in a postcondition must be declared const
//match_regex:line 27: error: value parameter "e" used in a postcondition must be declared const
//match_regex:line 30: error: value parameter "f" used in a postcondition must be declared const
//match_regex:line 32: error: value parameter "g" used in a postcondition must be declared const
//match_regex:line 33: error: value parameter "h" used in a postcondition must be declared const
//match_regex:line 34: error: value parameter "i" used in a postcondition must be declared const
// [dcl.contract.func]: a non-reference parameter that a postcondition
// odr-uses must be const on all declarations of the function, also those
// without the postcondition, named or not.  A parameter of array type is a
// pointer that is not const.  In a template, a parameter whose type is not
// dependent is checked in the template itself, instantiated or not (as GCC
// does); one whose type is dependent is checked in the instances.  An
// explicit instantiation's parameters are not checked.
int r1(const int a) post(a > 0);
int r1(int a);
int r1(const int a) { return a; }
void r2(const int b) post(b > 0);
void r2(int);
struct X { friend void r2(int c); };
void r3(const int d[]) post(d != 0);
void r4(int * const e) post(e != 0);
void r4(int e[]);
int r5(const int) post(true);
int r5(int) post(true);                          // OK, not used
template <class T> void t1(int f) post(f > 0) {}
template <class T> void t2(const int g) post(g > 0);
template <class T> void t2(int g);
template <class T> void t3(T h) post(h > 0) {}  // Error in t3<int>
template <class T> struct C { void m(int i) post(i > 0); };
template <class T> void t4(T j) post(j > 0) {}
template void t4<const int>(int);                // OK
template void t3<int>(int);
