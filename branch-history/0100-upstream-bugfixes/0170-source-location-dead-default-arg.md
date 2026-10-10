---
id: 0170-source-location-dead-default-arg
subject: 'Fold source location builtins in a default argument of a call in dead code []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A fix to upstream EDG, independent of contracts (though P3290's
contract-integrated `assert(1 == 1)` needs it).  An immediate invocation in
the unselected operand of a conditional whose condition is a constant --
`1 == 1 ? (void)0 : g(std::source_location::current())` -- was rejected:
"call to consteval function did not produce a valid constant expression",
with "expression cannot be interpreted" at the default argument
`__builtin_source_location()`.  The operand is potentially evaluated, so the
call is still an immediate invocation, evaluated with its default arguments
at the call site (g++ and Clang accept it).  `copy_default_arg_expr` copies
the default argument of such a call with `CE_COPY_NOT_EVALUATED` instead of
`CE_COPYING_EVALUATED_DEFAULT_ARG_EXPR` (so that dead code records no
destructions, fe76cea011), and only the latter made `i_copy_expr_tree` fold
the `enk_const_eval_deferred` node wrapping the builtin for the call site.
A new copy option, `CE_COPYING_DEAD_DEFAULT_ARG_EXPR`, set with
`CE_COPY_NOT_EVALUATED` for a potentially evaluated call that is not
evaluated, makes the fold happen there too; destructions and instantiations
in dead code are unchanged.

Tracked in `bug-reports/` (EDG-66) until upstream fixes it.

## Compile gap

None.

## Contents

- src/il.c : /CE_COPYING_DEAD_DEFAULT_ARG_EXPR/
- src/il.h : /CE_COPYING_DEAD_DEFAULT_ARG_EXPR/
- tests/tests/contracts/sema/*source_location_unselected_operand* : *
- tests/expectations/edg_x86_64/contracts.log : /^sema\/source_location_unselected_operand/
