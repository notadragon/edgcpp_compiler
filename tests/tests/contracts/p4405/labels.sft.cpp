//remark:contracts: P4405 with P3400 labels, as GCC: prove and diagnose are at ignore's level, where ignore is tried first, then diagnose, then prove (a label allowing only them takes enforce to diagnose); from prove, a label without prove takes it to diagnose, one with neither to ignore, then up; compute_semantic may give them when allowed, and a value that is no semantic (0, 10) is an error; in constant evaluation prove is enforce and diagnose observe
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=enforce -DFROM_PROVE=0:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=prove -DFROM_PROVE=1
//match_regex:line ([0-9]+): (error|warning): (no valid evaluation semantic for contract assertion|compute_semantic result is not in the allowed evaluation semantics|contract predicate is false in constant expression|static assertion failed|expression must have a constant value)
#include <contracts>
using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;
using enum evaluation_semantic;

template <evaluation_semantic... S>
struct allow {
  using assertion_control_object = allow;
  static constexpr evaluation_semantic_set allowed_semantics{S...};
};
template <evaluation_semantic R>
struct to {
  using assertion_control_object = to;
  constexpr evaluation_semantic compute_semantic(evaluation_semantic) const
  { return R; }
};
template <evaluation_semantic R>
struct to_excluded {
  using assertion_control_object = to_excluded;
  static constexpr evaluation_semantic_set allowed_semantics{enforce, observe};
  constexpr evaluation_semantic compute_semantic(evaluation_semantic) const
  { return R; }
};

#if !FROM_PROVE
// From enforce: a label allowing only prove and diagnose takes diagnose
// (through edg-gxx, g++ then diagnoses it).
constexpr int a(int x) pre<allow<prove, diagnose>{}>(x > 0) { return x; }
// compute_semantic giving prove or diagnose, allowed: in constant
// evaluation, enforce (b's static_assert fails) and observe (c's warns).
constexpr int b(int x) pre<to<prove>{}>(x > 0) { return x; }
constexpr int c(int x) pre<to<diagnose>{}>(x > 0) { return x; }
// Not allowed, or no semantic at all: errors.
constexpr int d(int x) pre<to_excluded<prove>{}>(x > 0) { return x; }
constexpr int e(int x) pre<to_excluded<diagnose>{}>(x > 0) { return x; }
constexpr int g(int x) pre<to<evaluation_semantic(10)>{}>(x > 0) { return x; }
constexpr int h(int x) pre<to<evaluation_semantic(0)>{}>(x > 0) { return x; }
static_assert(b(-1) == -1);
static_assert(c(-1) == -1);
#else
// From prove: with diagnose but not prove, diagnose, which constant
// evaluation uses as observe (a warns); with neither, ignore; with only
// enforce, up to enforce, whose false predicate fails the constant
// evaluation; with prove, prove, which constant evaluation uses as enforce
// (c fails too).
constexpr int a(int x) pre<allow<diagnose, ignore, enforce>{}>(x > 0) { return x; }
constexpr int b(int x) pre<allow<ignore, enforce>{}>(x > 0) { return x; }
constexpr int c(int x) pre<allow<prove>{}>(x > 0) { return x; }
constexpr int d(int x) pre<allow<enforce>{}>(x > 0) { return x; }
static_assert(a(-1) == -1);
static_assert(b(-1) == -1);
static_assert(c(-1) == -1);
constexpr int vd = d(-1);
int use_vd() { return vd; }
#endif
