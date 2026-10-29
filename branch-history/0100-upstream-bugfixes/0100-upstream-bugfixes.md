---
id: 0100-upstream-bugfixes
subject: 'Do not evaluate the operand of an assumption by inlining a call in it []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A fix to upstream EDG, independent of contracts.  With the C-generating back
end, `lower_statement` lowered the operand of `[[assume(E)]]` with
`lower_full_expr(E, statement)`, although `lower_full_expr` takes a statement
only when the expression is that statement's own.  With one passed, a call at
the top of `E` could be inlined by replacing the assumption's empty statement
with the inlined code: the operand, which is never evaluated, ran at run time
with its side effects, and the attribute was dropped (in a Debug build, the
file-scope entry referring to its operand was then left unwritten: "not all
expected entries were read").  No statement is passed now; a call in the
operand can still be inlined within the expression.

Tracked in `bug-reports/` (EDG-52) until upstream fixes it.

## Compile gap

None.

## Contents

- src/lower_il.c : /lower_full_expr\(expr, \(a_statement_ptr\)NULL\)|The operand is not the expression of the statement/
