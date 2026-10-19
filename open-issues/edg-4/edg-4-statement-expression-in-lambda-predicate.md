# EDG-4: a statement expression in a precondition or postcondition is not checked by the C-generating back end

**Status:** Deferred by the user 2026-10-05 (DECISIONS.md E27.3 in
notadragon_wg21); this branch only.
**Component:** statements.c `make_contract_check_statement` (the check
copies the predicate into the definition with `copy_contract_predicate`);
il.c `i_copy_expr_tree`, which does not copy statements.
**Found:** M1b, 2026-09-30; narrowed 2026-10-02 (statement expressions in a
lambda's predicates now work in the front end, in constant evaluation and
through edg-gxx).
**Test:** `tests/tests/contracts/codegen/statement_expression_unsupported.sft.cpp`
pins the diagnostic.

## Reproducer

    int f() { auto l = [](int x) pre(({ x > 0; })) { return x; }; return l(1); }

    cpfe --c++26 --g++ --contract_evaluation_semantic=quick_enforce t.cpp
    error: a statement expression in a precondition or postcondition is not yet supported

## Notes

A statement expression is allowed only in a lambda's predicate (the lambda
is in a function), as in GCC; a `contract_assert`, whose predicate is not
copied, is unaffected.  When the outlined body's checks evaluate the
predicate where it was scanned (M13c2), no copy is needed.  Our GCC ICEs on a statement
expression declaring a variable in a lambda's predicate (its GCC-632).
