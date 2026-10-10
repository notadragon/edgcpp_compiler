# EDG-113: no warning for a declaration with a dynamic selector's name and the wrong type

**Status:** Open; this branch only (contracts are not in upstream EDG).
**Component:** contract_config.c and declaration processing; band 2400.
**Found:** M12-EDG, 2026-10-09, mirroring GCC's and Clang's
`p3595-dynamic-selector-wrong-type` (EDG-103, EDG-104).
**Watch test:** `tests/tests/contracts/openbugs/edg-113-dynamic-selector-wrong-type/`.

## Reproducer

With a configuration whose "C++"-linkage dynamic output is named
`ns::sel`, a translation unit declaring `int ns::sel();` (or a variable
`ns::sel`, in an inline namespace, at global scope, redeclared) compiles
silently.  DECISIONS.md E37 (a): GCC (GCC-672) and Clang (CLANG-655) warn
at such a declaration under `-Wcontract-configuration` (on by default),
whether or not a contract uses the selector; a function of that name with
parameters, a function template and a "C"-linkage name are not warned
about.  edg-gxx maps `-Wno-contract-configuration` to
`--diag_suppress=contract_configuration`, so the warning would join that
tag.
