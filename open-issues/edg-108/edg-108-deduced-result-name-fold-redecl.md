# EDG-108: a fold over a parameter pack in a deduced-return postcondition on a non-defining declaration fails to instantiate

**Status:** Open; this branch only (contracts are not in upstream EDG).
**Component:** the postconditions of functions with deduced return types,
scanned after the body (M10a/b, bands 1040/1080), for a declaration that
precedes the definition.
**Found:** M12-EDG, 2026-10-09, mirroring GCC's
`p3098-capture-deduced-return.C` (EDG-96, GCC-477).
**Watch test:** `tests/tests/contracts/openbugs/edg-108-deduced-result-name-fold-redecl.sft.cpp`.

## Reproducer

    template <class... T> auto g(const T... t) post(r: ((t > 0) && ...));
    template <class... T> auto g(const T... t) post(r: ((t > 0) && ...)) { return 0; }
    int main() { return g(1, 2); }

    "t.cpp", line 1: error: fold expression does not refer to a parameter pack

The error names the first declaration and comes at the instantiation.
Without the result name, with a declared return type, or with the
definition alone, it compiles.  GCC's test has it as a member template of a
class template with a P3098 pack capture (`post [...c = t.m] (r: ((c > 0)
&& ...))`), declared in the class and defined outside it.
