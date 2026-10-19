//remark:contracts: a violation in constant evaluation is a warning under observe, and the value stands
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options_all:--contract_evaluation_semantic=observe
//match_regex:line 13: warning: contract predicate is false in constant expression
//match_regex:line 19: warning: contract condition is not constant
//match_regex:note: and 2 more contract violations not shown
// The array bound is evaluated more than once, but its violation is
// reported once.  At most eight violations are reported per evaluation; a
// note gives the number of the others.
extern "C" int puts(const char *);

constexpr int f(int x) pre(x > 0) { return x; }
constexpr int y = f(-1);
static_assert(y == -1);
int arr[f(-2) + 3];
static_assert(sizeof(arr) == sizeof(int));
int not_constexpr(int x) { return x; }
constexpr int g(int x) pre(not_constexpr(x) > 0) { return x; }
constexpr int z = g(4);
static_assert(z == 4);

constexpr int m(int x)
  pre(x != 1) pre(x != 2) pre(x != 3) pre(x != 4) pre(x != 5)
  pre(x != 6) pre(x != 7) pre(x != 8) pre(x != 9) pre(x != 10) { return x; }
constexpr int many() {
  return m(1) + m(2) + m(3) + m(4) + m(5) + m(6) + m(7) + m(8) + m(9) + m(10);
}
static_assert(many() == 55);
