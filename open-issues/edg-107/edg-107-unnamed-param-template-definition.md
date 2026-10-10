# EDG-107: an unnamed non-const parameter in a function template's definition is not diagnosed

**Status:** Open; this branch only (contracts are not in upstream EDG).
**Component:** the check that a parameter a postcondition odr-uses is const
on every declaration (band 1070), for function template definitions.
**Found:** M12-EDG, 2026-10-09, mirroring Clang's
`postcondition-unnamed-param-diagnostic.cpp` (EDG-95).
**Watch test:** `tests/tests/contracts/openbugs/edg-107-unnamed-param-template-definition.sft.cpp`.

## Reproducer

    template <class T> void t(const T b) post(b > 0);
    template <class T> void t(T) {}
    template void t<int>(int);

No diagnostic.  [dcl.contract.func]: a parameter a postcondition odr-uses
must be const in every declaration; in `t<int>` the definition's parameter
is a non-const `int`.  EDG does diagnose it when the definition names the
parameter (`template <class T> void t(T b) {}`), and when the declaration
that leaves it unnamed is not a definition ("an unnamed value parameter used
in a postcondition must be declared const"); Clang diagnoses the
definition (CLANG-640's test).
