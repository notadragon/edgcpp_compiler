//remark:contracts: P4283 requires-clauses are only allowed on templated functions (diagnosed, and the assertion dropped, as GCC does), need an operand after them, and must be the same on every declaration
//type:fn
//options_all:--contracts_p4283 --contract_evaluation_semantic=quick_enforce
//match_regex:line 22: error: requires clause on contract assertion only allowed on templated functions
//match_regex:line 23: error: requires clause on contract assertion only allowed on templated functions
//match_regex:line 24: error: requires clause on contract assertion only allowed on templated functions
//match_regex:line 26: error: requires clause on contract assertion only allowed on templated functions
//match_regex:line 28: error: requires clause on contract assertion only allowed on templated functions
//match_regex:line 29: error: requires clause on contract assertion only allowed on templated functions
//match_regex:line 31: error: requires clause on contract assertion only allowed on templated functions
//match_regex:line 33: error: requires clause on contract assertion only allowed on templated functions
//match_regex:line 34: error: requires clause on contract assertion only allowed on templated functions
//match_regex:line 38: error: mismatched requires clause on contract assertion in declaration
//match_regex:line 42: error: mismatched requires clause on contract assertion in declaration
//match_regex:line 44: error: mismatched requires clause on contract assertion in declaration
//match_regex:line 46: error: expected a "\("
//match_regex:line 49: error: requires clause on contract assertion only allowed on templated functions
//match_regex:line 50: error: requires clause on contract assertion only allowed on templated functions
// (No system headers: also runs under the C-generating back end.)
template <class T> concept C = sizeof(T) <= 4;
template <class T> concept D = true;
void f(int x) pre requires(true) (x > 0);             // Error
void f2(int x) pre requires C<int> (x > 0)            // Error
               pre requires C<int> (x > 1);           // Error
int f3(const int x)
  post requires (true) (r: r > x) { return x; }       // Error
void g(int x) {
  contract_assert requires(true) (x > 0);             // Error
  contract_assert requires C<int> (x > 0);            // Error
}
auto lam = [](int x) pre requires C<int> (x > 0) {};  // Error
struct S {
  void m(int x) pre requires C<int> (x > 0) {}        // Error
  void n(int x) { contract_assert requires C<int> (x > 0); }  // Error
};
template <class T> void r1(T x) pre requires C<T> (x > 0);
template <class T> void r1(T x) pre requires C<T> (x > 0);   // OK
template <class T> void r1(T x) pre requires D<T> (x > 0);   // Error
template <class T> void r2(T x) pre requires (C<T>) (x > 0);
template <class T> void r2(T x) pre requires (C<T>) (x > 0) {}  // OK
template <class T> void r3(T x) pre requires C<T> (x > 0);
template <class T> void r3(T x) pre (x > 0) {}               // Error
template <class T> void r4(T x) pre (x > 0);
template <class T> void r4(T x) pre requires C<T> (x > 0);   // Error
template <class T>
int a(T x) pre requires (sizeof(T) > 0);                     // Error
void h(int x) pre (x > 0);                                   // OK
// Dropped: no error for the non-const parameter, and no check.
void d1(int x) post requires (true) (x > 0);                 // Error
constexpr int d2(int x) pre requires (true) (x > 0) { return x; }  // Error
static_assert(d2(-1) == -1);                                 // OK
