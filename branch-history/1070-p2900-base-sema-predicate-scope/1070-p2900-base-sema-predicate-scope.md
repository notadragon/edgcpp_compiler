---
id: 1070-p2900-base-sema-predicate-scope
subject: 'contracts: P2900: predicate scope: constification, odr-use and the function''s name []'
depends: [1040-p2900-base-parse]
regenerates: []
fixes: []
---

## Rationale

What a contract predicate may name, and what naming it means.  This is
split out from 1060-p2900-base-sema-core because it is a different question:
that commit asks whether a specifier is well-formed and agrees with the
other declarations; this one, what an identifier in a predicate denotes.

Constification is the centre of it.  In a predicate an id-expression naming
a variable, a parameter, a structured binding or a result name is a const
lvalue (`constify_contract_predicate_operand` wraps it in a compiler-generated
`eok_lvalue_adjust`), and `this` points to const (`contract_predicate_this_type`,
applied in `variable_this_exists_full`, so that overload resolution sees the
const object, and in `make_this_variable_operand`).  The expression-stack
flag `in_contract_predicate`, unlike 1040's `is_contract_predicate`, is
inherited by the predicate's subexpressions.  decltype of such a name looks
through the adjustment, so it is still the declared type.  A lambda that
appears in a predicate is recorded (`note_lambda_in_contract_predicate`); in
its body a variable captured by reference, and the object `this` points to,
are const too (Clang's reading; EDG-26 records that our GCC leaves `*this`
alone there) -- but not a mutable lambda's own copy of `*this`, a member of
its closure object (`contract_this_is_lambda_copy`,
`variable_this_exists_for_copy`), which GCC and Clang let it modify.

A parameter that a postcondition odr-uses, unless a reference, must be
declared const.  The predicate is walked for its parameter references
outside unevaluated operands (`for_each_postcondition_param_use`): when it is
scanned, against the declaration's parameters
(`check_function_contract_predicate`), and at the start of a definition,
against the definition's parameter variables
(`check_postcondition_params_of_definition`), each parameter diagnosed once.
A coroutine's postconditions cannot odr-use its parameters, checked when the
function becomes a coroutine.  In a constructor's precondition or a
destructor's postcondition, a member named without `this->` (an implicit,
compiler-generated `this`) is an error.

A function's own name is in scope in its contract assertions: the operands
of a function declared by an unqualified name outside a class are cached
(`contract_operands_wait_for_declaration`) and scanned once `decl_routine`
has declared it (`scan_contract_operands_of_declaration`), with access
checked as from the function, as for its default arguments.

A lambda in a precondition or postcondition captures the declared
function's parameters, `this` and the result name as a lambda in a body
captures an enclosing function's entities.  A parameter has no variable
outside the body, so a lambda body (or capture list) naming one uses a proxy
instead: a variable owned by the specifier (`param_proxies`, made on demand
by `contract_param_proxy`), with the parameter's name and type, whose
initializer is the parameter reference that identifies the parameter.  The
variables a specifier owns -- proxies, the result name, P3098's captures --
are flagged `is_contract_specifier_var`: the capture code finds them in the
function parameter scope (`scope_depth_for_capture`), and captures them like
local variables of an enclosing function, implicitly or explicitly, by copy
or by reference, through nested lambdas, with the usual diagnostics (also
where the lambda is not in a function at all, `bad_nested_function_variable_ref`);
they never have a constant value.  `this` is captured as in a default member
initializer (`is_param_ref_capture`), found in the predicate's context
(`this_exists_around_contract_lambda`).  A generic lambda's call operator is
instantiated while the predicate is being scanned, in its context
(`in_contract_predicate_of_lambda`).  A lambda capturing a value parameter
in a postcondition odr-uses it, so the parameter must be const, and it
counts as used (`find_postcondition_param_capture`,
`mark_contract_param_capture`); and in a lambda in a constructor's
precondition or a destructor's postcondition, a member named without
`this->` is an error, as in the predicate (`in_lambda_in_cdtor_contract`).
A lambda's own precondition or postcondition whose lambda captures one of
its parameters keeps its predicate in the function's memory, like one that
names its captures (`expr_has_local_capturing_lambda`).

The tests are `tests/tests/contracts/sema/{constification,constification_ok,
lambda_captures,postcondition_params,postcondition_coroutine,cdtor_this,
own_name,lambda_declaration_entities,lambda_declaration_entities_errors,
lambda_this_copy_mutable}`.

The compound statement of a function-try-block is processed as a definition's
body (the rules for the parameters its postconditions use).

## Compile gap

Code of earlier bands refers to what this commit defines:
1040-p2900-base-parse's `scan_contract_specifier_operand` makes the
specifier the owner of the parameter proxies while its operand is scanned
(`set_contract_param_proxy_owner`), `declare_contract_result_name` flags the
result name `is_contract_specifier_var`, and `place_contract_predicate`
keeps a predicate with a local capturing lambda in the function
(`expr_has_local_capturing_lambda`).  All of these are defined here.

## Contents

- src/exprutil.c : #0, #1, #3, #7
- src/exprutil.h : #0, #1, #2, #4
- src/expr.c : #9, #18, #19, #20, #30, #39, #40, #41, #42, #43, #44, #48, #16, #17, #47, #57, #58, #26, #27, #28, #29, #31, #32, #33, #34, #45
- src/overload.c : #0, #1, #2, #3, #4, #5, #7, #8
- src/overload.h : *
- src/expr.h : #3:0
- src/scope_stk.c : #7, #8, #9
- src/templates.c : #13, #14, #15
- src/scope_stk.h : #5
- src/class_decl.c : /note_lambda_in_contract_predicate/, #18, #19, #22, #23, #38, #40, #13, #14, #15, #20, #21, #37
- src/il_alloc.c : #10, #11, /is_contract_specifier_var/
- src/il_def.h : #15, #16, /is_contract_specifier_var/
- src/il_display.c : /is_contract_specifier_var/
- src/declarator.c : #12, /^  scan_cached_contract_specifiers\(rp, dps->contract_specifiers,$/, /^\}  \/\* scan_contract_operands_of_declaration \*\/$/
- src/declarator.h : #2
- src/statements.c : /check_postcondition_params_of_definition/, #13:2
- src/func_def.c : /check_postcondition_params_of_definition/
- src/error_msg.txt : /(^|;)ec_contract_(post_param_not_const|post_param_in_coroutine|implicit_this_in_cdtor|post_unnamed_param_not_const|only_implicit_capture|only_implicit_this_capture|statement_expression_dependent_lambda|predicate_await);/
- src/Changes : @warning:1, @is:0
- src/Changes : /^Contracts \(P2900\): what a contract predicate names$/
- src/Changes : /^In a contract predicate an id-expression naming a variable \(including a$/
- src/Changes : /^parameter, a structured binding and a postcondition's result name\) is a$/
- src/Changes : /^const lvalue, for a reference the object it refers to, and "this" points to$/
- src/Changes : /^const, so a predicate cannot modify them; overload resolution sees the const$/
- src/Changes : /^                                \/\/ lvalue$/
- src/Changes : /^declared const \("value parameter used in a postcondition must be declared$/
- src/Changes : /^const"\), on the declaration with the postcondition and on the definition;$/
- tests/tests/contracts/sema/*constification* : *
- tests/tests/contracts/sema/*function_try_block_post_param* : *
- tests/tests/contracts/sema/*lambda_captures* : *
- tests/tests/contracts/sema/*contract_only_capture* : *
- tests/tests/contracts/sema/*generic_lambda_capture* : *
- tests/tests/contracts/sema/*statement_expression_lambda* : *
- tests/tests/contracts/sema/*postcondition_* : *
- tests/tests/contracts/sema/*cdtor_this* : *
- tests/tests/contracts/sema/*own_name* : *
- tests/tests/contracts/sema/*predicate_await* : *
- tests/tests/contracts/sema/*param_used_in_predicate* : *
- tests/tests/contracts/sema/*lambda_declaration_entities* : *
- tests/tests/contracts/sema/*lambda_this_copy_mutable* : *
- tests/expectations/edg_x86_64/contracts.log : /^sema\/(?!param_cv_member_definition|constexpr_|source_location_unselected_operand|c23_va_start|result_name_deduced)/
