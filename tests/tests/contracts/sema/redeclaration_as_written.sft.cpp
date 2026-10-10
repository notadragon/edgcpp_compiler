//remark:contracts: redeclaration predicates are compared as written, not as folded
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//match_regex:line 14: error: mismatched contract condition in declaration
//match_regex:line 16: error: mismatched contract condition in declaration
//match_regex:line 18: error: mismatched contract condition in declaration
//match_regex:line 27: error: mismatched contract condition in declaration
//match_regex:line 35: error: mismatched contract condition in declaration
// [dcl.contract.func]: the predicates of corresponding function contract
// specifiers are the same only if they are the same token sequence, up to
// the renaming of parameters and result names (formerly EDG-41).
constexpr int z = 0;
void f(int x) pre(x > 0);
void f(int x) pre(x > 1 - 1) {}               // Error, folds to the same
void g(int x) pre(x > 0);
void g(int x) pre(x > z) {}                   // Error, a named constant
void h(int x) pre(x > 1 - 1);
void h(int x) pre(x > 2 - 2) {}               // Error
void i(int x) pre(x > 1 - 1);
void i(int y) pre(y > 1 - 1) {}               // OK
void j(int x) pre(x > z);
void j(int y) pre(y > z) {}                   // OK
int k(const int x) post(r: r > (int)sizeof(int) && x != z + 1);
int k(const int y) post(s: s > (int)sizeof(int) && y != z + 1) { return 8; }
enum E { e0 };
void m(int x) pre(x != e0);
void m(int x) pre(x != 0) {}                  // Error, an enumerator
void n(int x) pre(x != e0);
void n(int x) pre(x != e0) {}                 // OK

// Templates.
template <class T> void t1(T x) pre(x > 1 - 1);
template <class T> void t1(T x) pre(x > 1 - 1) {}   // OK
template <class T> void t2(T x) pre(x > 1 - 1);
template <class T> void t2(T x) pre(x > 0) {}       // Error
