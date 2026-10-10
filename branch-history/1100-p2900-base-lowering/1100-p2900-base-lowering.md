---
id: 1100-p2900-base-lowering
subject: 'contracts: P2900: check contract assertions []'
depends: [1040-p2900-base-parse]
regenerates: []
fixes: []
---

## Rationale

Generate the checks of the contract assertions parsed by
1040-p2900-base-parse, for the C-generating back end.  The C++-generating
back end puts contract assertions out as written for the compiler of its
output (1020-p2900-base-il), and any other back end receives them in the
IL; `CONTRACT_CHECKS_IN_FRONT_END` (host_envir.h, from 1000-p2900-base-basic)
is TRUE exactly for the C-generating back end.  There the front end builds
each check as ordinary IL, which the back end lowers like any other
statement.

A check is `if (!predicate) call;`, a compiler-generated `stmk_if`.  That
back end accepts only ignore, which generates no check, and quick_enforce,
whose call is `__builtin_trap` (an extern "C" function predeclared in its
place, `enter_contract_check_routines`, where it is not a builtin).  An
exception from the evaluation of the predicate is a violation too: where
exceptions are enabled and the predicate might throw (`expr_might_throw`),
the check is in a try block whose `catch (...)` makes that call
(`make_contract_try_block`), and the predicate's full-expression
lifetime becomes a child of the try block's, so that the exception destroys
its temporaries and goes no further.  Constant evaluation skips such a try
block, as it skips the check, and finds a `contract_assert`'s predicate
through it.

A `contract_assert` is checked where it appears: its predicate is already in
the body, so it moves into its check (`make_contract_check_statement`).  The
checks of a definition's preconditions and postconditions are made as its
body starts (`prepare_contract_checks_for_lowering`) and kept on its function
scope (`contract_prologue`, `contract_epilogue`) for IL lowering, which puts
them into the lowered function once its body is lowered
(`add_contract_prologue_and_epilogue`): the preconditions' at its start,
ahead of a constructor's initialization of its bases and members, and the
postconditions' in an epilogue at its end, after its local variables, and a
destructor's members and bases, are destroyed.  Every return (the return
memo list) stores the returned value and branches to the epilogue, which
returns it; the postconditions name as the result a variable
(`contract_result_variable`, an `is_contract_result` temporary in the
function's scope) that names the object returned: the value itself for a
scalar, and for a reference or class return type a reference to the object,
set from the returned pointer, the hidden return-value parameter or the
returned temporary.  A coroutine is known as such only later, when its body
is transformed, which moves its precondition checks into the body
(`generate_coroutine_body`); its postconditions' stay on the scope, unused
by lowering (EDG-84).

A postcondition whose operand waits for a deduced return type (see
1040-p2900-base-parse) holds places in the prologue and epilogue, filled at
the end of the body, before lowering (`add_deferred_postcondition_checks`).

The predicate of a precondition or postcondition was scanned in the
declaration's parameter scope, so it is copied (`copy_contract_predicate`, a
new `copy_expr_tree` option) with each parameter reference replaced by the
definition's parameter variable and the result name by the result variable
(through a reference indirection where that is a reference,
`substitute_contract_result_reference`); a lambda in it captures the
parameter variable a proxy stands for, the result variable and the
definition's `this` (`substitute_contract_capture`).  A statement expression,
which cannot be copied, is reported as not yet supported in a precondition
or postcondition.

Checks are generated alike for member functions, friends and the instances
of templates.  A copied predicate forgets the recorded form of an unqualified
reference to a class member (`forget_unqualified_member_name_reference`), so
that a back end that reconstructs source qualifies the name where the check
needs it -- in a friend defined outside its class, say -- and
`__builtin_trap` is looked up at file scope, where a class scope cannot hide
it.

A reviewer should check the epilogue's handling of returns in
`add_contract_prologue_and_epilogue`, in particular the class cases (a value
returned through the hidden parameter, and one returned in a temporary), and
that the EH function prologue, added after it, sees only the epilogue's
return.

The tests are in `tests/tests/contracts/codegen`: quick_enforce and ignore
under both configurations, the checks of templates (quick_enforce, so that
the C-generating back end runs EDG's instances), the order of the checks of
constructors and destructors, the result object and the returns reaching the
epilogue, a throwing predicate, the unsupported cases and the checks as displayed in the IL
(C-generating only); and, through g++, the contract assertions the
C++-generating back end puts out (`pass_through`), each semantic, a
user-defined violation handler, and member functions and friends.

The checks of a definition whose body is a function-try-block are made at
the start of its compound statement, and lowering puts them outside the try
block.

## Compile gap

None.

## Contents

- src/statements.c : /#include "(sys_predef|il_walk)\.h"/, #4, #10, @__builtin_trap:0, @__builtin_trap:2, #11:13
- src/statements.c : /add_precondition_checks\(\);/
- src/il.c : #12, #15, /^  if \(options & CE_SUBSTITUTE_CONTRACT_NAMES\) \{$/
- src/il.h : /CE_SUBSTITUTE_CONTRACT_NAMES|copy_contract_predicate/
- src/il_def.h : /is_contract_result/, #17
- src/il_alloc.c : /is_contract_result/, #8
- src/il_display.c : /is_contract_result/, #8
- src/walk_entry.h : #2
- src/statements.h : #1
- src/lower_il.c : #4, #5
- src/func_def.c : #0
- src/interpret.c : #16, #17:1
- src/symbol_tbl.c : #2:0, #2:2, #6
- src/symbol_tbl.h : #4, #6
- src/fe_init.c : /enter_contract_check_routines/
- src/error_msg.txt : /(^|;)ec_contract_(postcondition_return_unsupported|quick_enforce_needs_builtin_trap|statement_expression_unsupported);/
- src/Changes : @them:1, @their:0
- src/Changes : /^Contracts \(P2900\): checks of contract assertions$/
- src/Changes : /^Contract assertions on functions and lambdas, and contract_assert$/
- src/Changes : /^statements, are now checked\.  For the C-generating back end the front end$/
- src/Changes : /^generates the checks as ordinary code \(an "if" testing the predicate\) in the$/
- src/Changes : /^function definition -- preconditions as the function is entered,$/
- src/Changes : /^the predicate is not evaluated, and quick_enforce, for which a violation$/
- src/Changes : /^is generated as$/
- src/Changes : /^Each return branches to the postconditions' checks, which follow the$/
- src/Changes : /^constructor's preconditions are checked before its bases and members are$/
- src/Changes : /^The C\+\+-generating back end instead puts contract assertions out as written$/
- tests/expectations/edg_x86_64/contracts.log : /^codegen\//
- tests/expectations/edg_x86_64_gxx/contracts.log : /^(codegen|constexpr)\//
- tests/tests/contracts/codegen/* : *
