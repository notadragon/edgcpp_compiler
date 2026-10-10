# EDG-106: a lambda in a function template's precondition or postcondition naming the parameter pack is an internal error

**Status:** Open; this branch only (contracts are not in upstream EDG).
**Component:** the lambdas of a declaration's predicates (M10c, band 1070:
specifier-owned parameter proxies), for a function parameter pack;
scope_stk.c `find_parameter_for_pack` asserts.
**Found:** M12-EDG, 2026-10-09, mirroring GCC's
`predicate-lambda-pack-capture.C` (EDG-93, GCC-656).
**Watch test:** `tests/tests/contracts/openbugs/edg-106-predicate-lambda-pack.sft.cpp`.

## Reproducer

    template <class... T>
    int g(T... xs) pre([=] { return ((xs > 0) && ...); }()) { return 0; }
    int main() { return g(1, 2); }

    internal error: assertion failed at: "scope_stk.c", line 12193 in find_parameter_for_pack

Also a postcondition (`[&]`), `sizeof...(xs)` in the lambda, and the
explicit `[xs...]`, which first reports a bogus '"xs" has already been
declared in the current scope'.  The same lambda in a `contract_assert` in
the body compiles.
