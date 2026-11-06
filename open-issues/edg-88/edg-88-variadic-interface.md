# EDG-88: a virtual call of a C-variadic function has no interface checks under the C-generating back end

**Status:** Deferred by the user 2026-10-05; this branch only (contracts are
not in upstream EDG).
**Component:** func_def.c `make_contract_interface_wrapper` (which makes no
wrapper for a function with an ellipsis).
**Found:** M13c2, 2026-10-05, implementing EDG-71.
**Watch test:** `tests/tests/contracts/openbugs/edg-88-variadic-interface.sft.cpp`.

## Reproducer

    struct B { virtual int f(int n, ...) pre (n >= 0) { return n; } };
    struct D : B { int f(int n, ...) override { return n; } };
    D d; B *p = &d; p->f(-1);

With the C-generating back end the call returns; our GCC traps on `B::f`'s
precondition (P3097: the statically chosen function's assertions are checked
around the final overrider's).

## Notes

The interface checks of a virtual call are made in a wrapper member that
passes its arguments on to the virtual call as they are; the arguments of an
ellipsis cannot be passed on in C.  GCC's wrapper passes them with
`__builtin_va_arg_pack` (an always-inline GNU wrapper, which the
C-generating back end cannot put out: see EDG-78); Clang checks such a call
at the call site, which is the way here: evaluate the object and the named
arguments once, check the preconditions, make the virtual call with the
variadic arguments, check the postconditions.
