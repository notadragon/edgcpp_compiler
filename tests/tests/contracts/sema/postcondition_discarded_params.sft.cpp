//remark:contracts: a parameter named by a discarded-value expression in a postcondition is not odr-used
//type:fn
//match_regex:line 26: error: value parameter "x" used in a postcondition must be declared const
//match_regex:line 27: error: value parameter "b" used in a postcondition must be declared const
//match_regex:line 29: error: value parameter "x" used in a postcondition must be declared const
//match_regex:line 30: error: value parameter "s" used in a postcondition must be declared const
//match_regex:line 31: error: value parameter "x" used in a postcondition must be declared const
//match_regex:line 32: error: value parameter "x" used in a postcondition must be declared const
//match_regex:line 33: error: value parameter "b" used in a postcondition must be declared const
// [basic.def.odr]: a non-reference parameter that is a potential result of a
// discarded-value expression to which the lvalue-to-rvalue conversion is not
// applied (the left operand of a comma, the operand of a cast to void) is not
// odr-used, so a postcondition naming only there needs no const
// ([dcl.contract.func]).  Lines 17 to 25 are OK; from line 26 on, each
// parameter is odr-used.
struct S { int m; bool ok() const; };
int f(int b) post(r: (b, r != 0)) { return 1; }
bool g(int x) post(((void)x, true)) { return true; }
bool h(int x) post((static_cast<void>(x), true)) { return true; }
void cls(S s) post((s, true)) {}
void mem(S s) post((s.m, true)) {}
int nest(int x, int y) post(r: ((x, y, true) && r == r)) { return x + y; }
int cond(const int c, int x, int y) post(r: ((c ? x : y), r == r)) { return c; }
template <class T> bool t(T x) post(((void)x, true)) { return true; }
bool use_t = t(1);
int both(int x) post(r: ((x, true) && x == r)) { return x; }
bool rhs(bool b) post((true, b)) { return b; }
bool use(int);
int arg(int x) post(r: ((use(x), true) && r == r)) { return x; }
void memcall(S s) post((s.ok(), true)) {}
void vol(volatile int x) post((x, true)) {}       // a volatile glvalue is read
void vol2(volatile int x) post(((void)x, true)) {}
int cnd(int b, const int x) post(r: ((b ? x : x), r == r)) { return x; }
