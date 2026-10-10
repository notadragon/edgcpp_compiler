# EDG-109: --contracts_p4299 predefines no vendor macro

**Status:** Open; this branch only (contracts are not in upstream EDG).
**Component:** macro.c (the predefined contract vendor macros, beside
`__gcc_contracts_p3290`); band 5000.
**Found:** M12-EDG, 2026-10-09, mirroring GCC's `p4299-feature-macro.C`
(EDG-98).
**Watch test:** `tests/tests/contracts/openbugs/edg-109-p4299-macro.sft.cpp`.

## Reproducer

    // --contracts_p4299, C++26
    static_assert(__gcc_contracts_p4299 > 0);    // "identifier is undefined"

Since their M6 milestones our GCC defines `__gcc_contracts_p4299` under
`-fcontracts -fcontracts-p4299` in C++ (and not without either), and Clang
defines `__clang_contracts_p4299` under `-fcontracts-p4299` (202610L, the
implemented revision's date).  EDG predefines none: band 5000's rationale still
says "as in GCC and Clang, ... predefines no macro", which is no longer
true.  The macro of the emulated compiler (GNU or Clang mode), as for P3290's.
