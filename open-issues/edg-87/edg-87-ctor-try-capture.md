# EDG-87: a capture of a constructor or destructor with a function-try-block is not destroyed on unwinding

**Status:** Deferred by the user 2026-10-05; this branch only (contracts are
not in upstream EDG).
**Component:** statements.c `protect_contract_captures` (which skips such
bodies).
**Found:** M13c2, 2026-10-05, fixing EDG-86.
**Watch test:** `tests/tests/contracts/openbugs/edg-87-ctor-try-capture.sft.cpp`.

## Reproducer

    struct S {
      S(const Box &b) post [c = b] (c.v == 1)
      try { throw 1; } catch (int) { puts("handler"); }
    };

With the C-generating back end (quick_enforce), constructing an `S` prints
"capture copied", "handler", and the exception reaches the caller with the
capture undestroyed; our GCC destroys it between the handler and the
caller's catch.  A function-try-block of any other function, and every other
body, is protected (EDG-86).

## Notes

`protect_contract_captures` puts a body in a try block whose handler destroys
the captures and rethrows; a function-try-block is first put in a block of
its own.  A constructor's or destructor's function-try-block must stay the
body: its lowering puts the mem-initializers (or the member and base
destructions) inside it and rethrows at the end of each handler.  A fix adds
to it a last handler `catch (...) { destructions; throw; }` and makes each
existing handler destroy the captures at its end and when it exits with an
exception (a try block around its body).
