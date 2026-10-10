//remark:contracts: EDG-106: a lambda in a function template's precondition or postcondition that names the function parameter pack is an internal error (find_parameter_for_pack)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//cases:3
// The recording is the current, wrong behavior: when this test deviates,
// the bug may be fixed.  Correct: each compiles, the lambda capturing the
// pack (as the same lambda in a contract_assert does).  The explicit
// capture also gets a bogus '"xs" has already been declared'.  From GCC's
// predicate-lambda-pack-capture.C (EDG-93).
#if TEST_NUMBER == 1
template <class... T>
int g(T... xs) pre([=] { return ((xs > 0) && ...); }()) { return 0; }
#elif TEST_NUMBER == 2
template <class... T>
int p(const T... xs) post([&] { return ((xs > 0) && ...); }()) { return 0; }
#else
template <class... T>
int g(T... xs) pre([xs...] { return ((xs > 0) && ...); }()) { return 0; }
#endif
int main() { return TEST_NUMBER == 2 ? p(1, 2) : g(1, 2); }
