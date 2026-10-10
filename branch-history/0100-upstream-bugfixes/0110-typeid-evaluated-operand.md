---
id: 0110-typeid-evaluated-operand
subject: 'Evaluate the operand of typeid of a polymorphic glvalue whose type is known []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A fix to upstream EDG, independent of contracts.  The operand of `typeid`
is evaluated when it is a glvalue of polymorphic class type
([expr.typeid]/3), so a variable it names is odr-used.  `scan_typeid_operator`
skips the run-time type lookup when the type of the complete object is known
(a variable of class type, `*this` in a constructor), and used the same flag,
`runtime_case`, to decide whether the operand is evaluated: in that case the
operand was scanned only as unevaluated, so the variable was not odr-used --
a lambda named it without capturing it, and a value parameter named that way
in a postcondition was not required to be const.  A separate flag,
`operand_evaluated`, now decides the scan (the second, evaluated scan; the
operand's object lifetime; what counts against a constant expression), and
`runtime_case` only whether the lookup is done at run time.  And where the
lookup is skipped, the C-generating back end now keeps the operand's side
effects (`lower_typeid`), which it had dropped.

Tracked in `bug-reports/` (EDG-58) until upstream fixes it.

## Compile gap

None.

## Contents

- src/expr.c : /operand_evaluated/
- src/lower_eh.c : *
- tests/tests/contracts/sema/*typeid_evaluated_operand* : *
- tests/expectations/edg_x86_64_gxx/contracts.log : /^sema\/typeid_evaluated_operand/
