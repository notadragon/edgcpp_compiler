---
id: 1040-p2900-base-parse
subject: 'contracts: P2900: parse contract specifiers and assertions []'
depends: [1000-p2900-base-basic, 1020-p2900-base-il]
regenerates: []
fixes: []
---

## Rationale

Parse the `contract_assert` statement and the `pre` and `post` function
contract specifiers of functions, member functions, templates and lambdas,
into the IL of 1020-p2900-base-il.

`pre` and `post` are context-sensitive: they are recognized only right after
a function declarator (and its trailing requires-clause), where an identifier
could not otherwise appear.  Each predicate is scanned with the function
parameter scope reactivated, as for a trailing requires-clause, and
separately for each specifier, so a postcondition's result name is visible
only in its own predicate.  The result name is a variable of the function's
return type.  A new entry point, `scan_contract_predicate`, scans a
potentially-evaluated conditional-expression converted to bool and marks its
expression-stack entry, so that a parameter used in a precondition or
postcondition is accepted and represented by an `enk_param_ref` (as in a
`decltype` in a declarator) rather than rejected.  It also records the
predicate's text, a string formed from its tokens, kept as they are fetched.

The contract assertions of a member function, and of a friend declared in a
class, are a complete-class context.  On such a declaration in a class
definition the operand of each specifier is cached instead of scanned, the
declaration's routine fixup is marked (`process_contracts`, beside
`process_exception_spec`), and the class's fixup pass scans the operands
with the class and the function parameter scope reactivated
(`scan_cached_contract_specifiers`), before any member function body.  A
template's predicates are scanned in the template and their tokens kept: at
declaration for a function template, and when the class is complete for a
member of a class template or a member template.
1080-p2900-base-sema-templates instantiates the specifiers of instances from
those tokens, so where an instance's are instantiated -- the rescan of a
template declaration, and an ordinary member of a class template's
instantiation -- the specifiers are skipped, as the trailing requires-clause
is (`contract_specifiers_are_skipped`).

The specifiers attach to the routine in `decl_routine`, `decl_member_function`
(lambdas included) and the template declaration functions; the first
declaration that has specifiers provides them, as redeclaration matching
comes later.  The operand of a postcondition that names the result of a
function with a deduced return type waits in its token cache until the body
has deduced the type (`scan_postconditions_awaiting_deduction`): it is
scanned after the function scope is popped, or, where the front end
generates the checks (and for a lambda, which names its captures), at the
end of the body before the pop, the function's memory region, object
lifetimes and "this" set aside except for a lambda.

A reviewer should check the unlinking of the result-name symbol after each
postcondition: the reactivated prototype-scope symbols are relinked into the
scope's list, so the result name ends up chained from the last of them and
would otherwise reappear when the function body reactivates them.

The tests are the first in `tests/tests/contracts/parse`: option handling,
the forms that parse, the errors (also in member functions, where a
predicate can name a member declared after it), and the IL as displayed by
the C-generating front end (the C++-generating one has no IL display), with
the ignore semantic so that no checks appear in it.

## Compile gap

The parsing code calls functions of 1060-p2900-base-sema-core:
`attach_contract_specifiers` (decls.c, class_decl.c),
`contract_result_name_hides_parameter`, `contract_result_name_void_error`,
`diagnose_deduced_contract_result_name`, `skip_function_contract_specifiers`
and `match_redeclared_contract_specifiers` (declarator.c, class_decl.c); and
of 1070-p2900-base-sema-predicate-scope: `check_function_contract_predicate`,
`contract_operands_wait_for_declaration` and
`scan_contract_operands_of_declaration` (declarator.c, decls.c), and the
`in_contract_predicate` flag set in `scan_contract_predicate`.  On its own
this commit would carry the earlier forms: specifiers assigned to the
routine when it has none, the result-name messages issued inline, the
deduced-return-type message at the result name, and a non-member's operands
scanned in its declarator.

## Contents

- src/declarator.c : #8, @enabled:0, @enabled:2, #10, #11, @contract_specifier_predicate:0, @contract_specifier_predicate:3, @contract_specifier_predicate:7, @operator:0, @operator:3, @operator:5, @none:0, @none:2, @none:4, @none:6, /^templ_specifiers\.  Give rp its own function contract specifiers, scanned from$/, /^    csp = alloc_contract_specifier\(templ_csp->kind\);$/, /^    if \(templ_csp->result_name != NULL && dps\.has_deducible_return_type\) \{$/, @next_specifier:0, @next_specifier:2, @instantiation:0, @instantiation:2
- src/declarator.h : #1:0, #1:2
- src/class_decl.h : #0
- src/templates.c : #2
- src/templates.h : /scan_member_template_contract_specifiers/
- src/decls.c : #0, #1:0, #1:2, #3, #4
- src/decls.h : #0
- src/class_decl.c : #0, #2, #5, #6, #24, #25, #27, #28, #29, #32, #34, #35, #36
- src/func_def.c : /scan_lambda_contract_operands/, /scan_postconditions_awaiting_deduction/
- src/statements.c : /^static void contract_assert_statement\(void\)$/
- src/statements.c : /#include "declarator\.h"/, #11:0, #11:2, #11:5, #11:7, #11:10, #11:12, #11:14, /case tok_contract_assert:/
- src/expr.c : #24, #46
- src/expr.h : #4
- src/error_msg.txt : /(^|;)ec_contract_result_name_(void_return|deduced_return_type);/
- src/Changes : @their:1, @leaves:1, @them:0
- src/Changes : /^Contracts \(P2900\): contract assertions of member functions$/
- src/Changes : /^Contract assertions on member functions, including static members,$/
- src/Changes : /^Contracts \(P2900\): options, contract_assert, pre and post$/
- src/Changes : /^The front end now parses contract assertions/
- src/Changes : /^P2900\): the contract_assert statement and the pre and post function/
- src/Changes : /^specifiers of functions and lambdas, including a postcondition's$/
- src/Changes : /^result name\.  They are recorded in the IL/
- src/Changes : /^kind, a contract_specifiers list on a_routine, and an stmk_contract_assert$/
- src/Changes : /^Contracts are enabled by default in C\+\+26 mode/
- src/Changes : /^The new --contract_evaluation_semantic=X option selects the evaluation$/
- src/Changes : /^semantic of every contract assertion: ignore, observe, enforce/
- tests/expectations/edg_x86_64/contracts.log : /^parse\//, /^sema\/result_name_deduced/
- tests/expectations/edg_x86_64_gxx/contracts.log : /^parse\//
- tests/tests/contracts/parse/* : *
- tests/tests/contracts/sema/*result_name_deduced* : *
