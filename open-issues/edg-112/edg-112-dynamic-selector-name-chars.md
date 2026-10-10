# EDG-112: a P3595 dynamic selector name that is not made of identifiers is accepted

**Status:** Open; this branch only (contracts are not in upstream EDG).
**Component:** contract_config.c (the `"dynamic"` output's `"name"`
check), band 2400.
**Found:** M12-EDG, 2026-10-09, mirroring GCC's
`p3595-dynamic-bad-name-chars.C` (EDG-101; GCC-671, CLANG-653).
**Watch test:** `tests/tests/contracts/openbugs/edg-112-dynamic-selector-name-chars.sft.cpp`.

## Reproducer

    edg-gxx -std=c++26 -fsyntax-only \
      '-fcontract-configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C++","name":"ns::sel<int>"}}}]' t.cpp

No diagnostic.  The check rejects only empty components of a "C++" name
(`a::::b`, a trailing `::`).  GCC and Clang reject, at configuration parse,
every name that is not identifiers joined by `::` (a "C" name: one
identifier): a template-id, an operator-function-id, a parameter list, a
component starting with a digit, a space, a qualified or empty "C" name
(DECISIONS.md E38).  `$` is accepted in identifiers.
