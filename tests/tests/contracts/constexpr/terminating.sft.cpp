//remark:contracts: a violation in constant evaluation is an error under a terminating semantic
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex:line 17: error: contract predicate is false in constant expression
//match_regex:line 22: error: contract predicate is false in constant expression
//match_regex:line 27: error: contract predicate is false in constant expression
//match_regex:line 34: error: contract predicate is false in constant expression
//match_regex:line 42: error: contract predicate is false in constant expression
//match_regex:line 49: error: contract condition is not constant
//match_regex:line 53: error: contract predicate is false in constant expression
// Also runs under the C-generating back end, which supports quick_enforce,
// and whose generated checks constant evaluation skips.  The diagnostic is
// at the contract assertion; a value whose evaluation violates one is an
// error, so the static_assert says nothing more.

// A precondition.
constexpr int pre1(int x) pre(x > 0) { return x; }
static_assert(pre1(1) == 1);    // OK
static_assert(pre1(0) == 0);    // Error, at the "pre"
// A postcondition naming the result.
constexpr int post1(const int x) post(r: r > x) { return x + 1; }
constexpr int post2(const int x) post(r: r > x) { return x; }
static_assert(post1(1) == 2);   // OK
constexpr int v1 = post2(3);    // Error, at the "post"
// contract_assert.
constexpr int assert1(int x) {
  contract_assert(x != 2);
  return x;
}
constexpr int v2 = assert1(2);  // Error, at the "contract_assert"
// A member function's precondition, through "this".
struct S {
  int v;
  constexpr int get(int d) const pre(v > d) { return v; }
};
constexpr S s{5};
static_assert(s.get(1) == 5);   // OK
static_assert(s.get(7) == 5);   // Error, at the "pre"
// A constructor's precondition.
struct C {
  int v;
  constexpr C(int a) pre(a > 0) : v(a) {}
};
constexpr int make_c(int a) { return C(a).v; }
static_assert(make_c(1) == 1);  // OK
static_assert(make_c(0) == 0);  // Error, at the "pre"
// A predicate that is not a core constant expression.
int not_constexpr(int x) { return x; }
constexpr int nc(int x) pre(not_constexpr(x) > 0) { return x; }
constexpr int v3 = nc(1);       // Error, at the "pre"
// The constant initialization of a variable of static storage duration is
// manifestly constant-evaluated: an error, not dynamic initialization.
constexpr int pre2(int x) pre(x > 0) { return x; }
int dynamic = pre2(0);          // Error, at the "pre"
// A local variable's initializer is not: no diagnostic; the precondition is
// checked at run time.
int local() {
  int x = pre1(0);              // OK
  return x;
}
