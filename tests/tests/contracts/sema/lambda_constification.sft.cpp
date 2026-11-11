//remark:contracts: in a lambda in a predicate, a variable declared outside the assertion is const
//type:fn
//require:BACK_END_IS_CP_GEN_BE 1
//match_regex:", line 23: error
//match_regex:", line 24: error
//match_regex:", line 25: error
//match_regex:", line 26: error
//match_regex:", line 27: error
//match_regex:", line 43: error
//match_regex:", line 44: error
//match_regex:", line 48: error
// Every Error names a variable declared outside the assertion, from within
// its predicate, at any depth of lambdas; every OK names a variable
// declared in the assertion, or a member of a closure that captures by
// copy.  decltype((x)) sees the const unless a lambda in the assertion
// captures x by copy.
template <class T, class U> constexpr bool same = false;
template <class T> constexpr bool same<T, T> = true;
int g_n = 0;
struct HasStatic { static int s; };
void f(int x) {
  static int local_static = 0;
  contract_assert([] { ++g_n; return true; }());
  contract_assert([] { ++HasStatic::s; return true; }());
  contract_assert([&] { ++local_static; return true; }());
  contract_assert([&] { return [&] { return ++x > 0; }(); }());
  contract_assert([&x] { return [&x] { return ++x > 0; }(); }());
  contract_assert([&] {
    static_assert(same<decltype((x)), const int &>);            // OK
    static_assert(same<decltype(x), int>);                      // OK
    return true;
  }());
  contract_assert([=]() mutable {
    static_assert(same<decltype((x)), int &>);                  // OK
    return true;
  }());
  contract_assert([](int k) { return ++k > 0; }(1));            // OK
  contract_assert([] { int k = 0; static int s; ++s; return ++k > 0; }());
  contract_assert([=]() mutable { return [&] { return ++x > 0; }(); }());
  contract_assert([&] { int j = 0; return [&] { return ++j > 0; }(); }());
}
// The result name, from a lambda in the postcondition.
int r1() post(r: [&] { return ++r > 0; }()) { return 1; }
int r2() post(r: [] { return ++g_n > 0; }()) { return 1; }
// this points to const in the bodies of nested lambdas, too.
struct T {
  int m;
  void h() { contract_assert([&] { [&] { ++m; }(); return true; }()); }
};
