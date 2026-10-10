//remark:contracts: EDG-70 (stock): with exceptions disabled the noexcept operator is true for every expression, where g++ (and Clang) still answer from the exception specifications
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options_all:--no_exceptions
//match_regex:line 13: error: static assertion failed
//match_regex:line 15: error: static assertion failed
//match_regex:line 19: error: static assertion failed
// Records the current (wrong) behavior: each of these assertions holds with
// g++ -fno-exceptions and fails here.  A fix shows up as a deviation.
void f();
void g() noexcept;
struct S { S(); S(const S &) noexcept; };
static_assert(!noexcept(f()));
static_assert(noexcept(g()));
static_assert(!noexcept(S()));
template<class T> constexpr bool nothrow_call = noexcept(T()());
struct F { void operator()(); };
struct G { void operator()() noexcept; };
static_assert(!nothrow_call<F>);
static_assert(nothrow_call<G>);
