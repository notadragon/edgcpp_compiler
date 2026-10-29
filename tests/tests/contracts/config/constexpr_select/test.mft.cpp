//remark:contracts: a configuration chooses the semantic of each assertion in constant evaluation (kind, namespace, location, constexpr)
//type:fn
//source_files:config.json
//options_all:--contract_evaluation_semantic=quick_enforce --contract_configuration_file=config.json
//match_regex:line 22: warning: contract predicate is false in constant expression
//match_regex:line 24: warning: contract predicate is false in constant expression
//match_regex:line 27: warning: contract predicate is false in constant expression
//match_regex:line 30: warning: contract predicate is false in constant expression
//match_regex:line 32: warning: contract predicate is false in constant expression
//match_regex:line 34: warning: contract predicate is false in constant expression
//match_regex:line 45: warning: contract predicate is false in constant expression
//match_regex:line 62: warning: contract predicate is false in constant expression
//match_regex:line 63: warning: contract predicate is false in constant expression
//match_regex:line 39: error: contract predicate is false in constant expression
//match_regex:line 44: error: contract predicate is false in constant expression
//match_regex:line 64: error: contract predicate is false in constant expression
// Under the catch-all (quick_enforce) a violation is an error; the entries
// make some observed (a warning) or ignored.  A dynamic entry without a
// semantic is passed over in constant evaluation, and "caller", "group",
// "implicit" and "constexpr": false entries never match there.
namespace obs {
  constexpr int f(int x) pre(x > 0) { return x; }       // observe
  namespace inner {
    constexpr int g(int x) pre(x > 0) { return x; }     // observe (nested)
  }
  namespace {
    constexpr int u(int x) pre(x > 0) { return x; }     // observe (unnamed)
  }
  struct C {
    constexpr int m(int x) const pre(x > 0) { return x; }  // observe
  };
  template<class T> constexpr T t(T x) pre(x > 0) { return x; }  // observe
  constexpr int lam(int x) {
    auto l = [](int y) pre(y > 0) { return y; };        // observe (lambda)
    return l(x);
  }
}
namespace obsx {
  constexpr int h(int x) pre(x > 0) { return x; }       // error
}
namespace ign {
  constexpr int k(int x) pre(x > 0) { return x; }       // ignored
}
constexpr int e(int x) pre(x > 0) { return x; }         // error
constexpr int a(int x) { contract_assert(x > 0); return x; }  // observe

constexpr int v1 = obs::f(0);
constexpr int v2 = obs::inner::g(0);
constexpr int v3 = obs::u(0);
constexpr int v4 = obs::C().m(0);
constexpr int v5 = obs::t(0);
constexpr int v6 = obs::lam(0);
constexpr int v7 = obsx::h(0);
constexpr int v8 = ign::k(0);
constexpr int v9 = e(0);
constexpr int v10 = a(0);
int use() { return v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v10; }




constexpr int l1(int x) pre(x > 0) { return x; }        // observe (location)
constexpr int l2(int x) pre(x > 0) { return x; }        // observe (location)
constexpr int l3(int x) pre(x > 0) { return x; }        // error
constexpr int w1 = l1(0) + l2(0) + l3(0);
int use2() { return w1; }
