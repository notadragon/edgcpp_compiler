# EDG-84: a coroutine's contract assertions are checked in the wrong place by the C-generating back end

**Status:** Deferred by the user 2026-10-05; this branch only (contracts are
not in upstream EDG).
**Component:** func_def.c `generate_coroutine_body` (where a coroutine's
precondition checks are moved into its body); statements.c
`prepare_contract_checks_for_lowering`.
**Found:** M13c1, 2026-10-05, in the C-generating sweep of the imports
(`imported/gcc/coroutine-pre-post.sft.cpp`).
**Watch test:** none possible yet: the C-generating back end cannot compile a
coroutine (below), and a front-end test could only pin the IL display.

## Reproducer

    task coro(int x) pre(x > 0) post(ok) { co_return; }

(with a `task` whose promise type suspends never).  `cpfe --c++26
--contract_evaluation_semantic=quick_enforce` puts the check of `x > 0` in the
coroutine body, after `co_await p.initial_suspend()`, on the coroutine-frame
copy of `x`; the postcondition gets no check.  P2900 checks a coroutine's
preconditions as it is called and its postconditions as it returns to its
caller, on the original parameters and the return object, as our GCC does.

## Notes

Unobservable at run time for now: the C-generating back end leaves `co_await`
unlowered (the generated C calls an undeclared
`co_await_ZN...initial_suspendEv`), with or without contracts, so no coroutine
program compiles through eccp.  The front end learns that a function is a
coroutine only at its first `co_await`, `co_yield` or `co_return`, after the
checks are made (`prepare_contract_checks_for_lowering`, as the body starts);
`generate_coroutine_body` then moves the precondition checks into the body,
where they were before M13c1, and drops the postconditions'.  The fix is to
leave both to IL lowering's prologue and epilogue, with the predicates naming
the original parameters rather than the copies `copy_coroutine_parameters`
substitutes.
