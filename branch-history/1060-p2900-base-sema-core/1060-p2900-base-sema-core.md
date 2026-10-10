---
id: 1060-p2900-base-sema-core
subject: 'contracts: P2900: attach and validate function contract specifiers []'
depends: [1040-p2900-base-parse]
regenerates: []
fixes: []
---

## Rationale

Attach the function contract specifiers of each declaration to its function
(`attach_contract_specifiers`) and check that they agree with the other
declarations of the function, and that they and their result names appear
only where P2900 allows them.  This answers "is this contract-specifier
well-formed and does it agree with the other declarations"; what an
identifier in a predicate denotes is 1070-p2900-base-sema-predicate-scope.

The first declaration provides the specifiers.  A redeclaration omits them
or repeats them: the same number, in the same order, each of the same kind,
with a result name on both or neither, and the same predicate as written
(`match_contract_specifiers`).  Predicates are compared with
`compare_expressions` as requires-clauses are, through
`equiv_contract_predicates` (il.c), which also makes the two result names
correspond; parameters are `enk_param_ref`s, compared by position, so a
renamed parameter still matches.  Adding specifiers is an error, except
after an error in a predicate.  An out-of-class member definition is
attached in `define_member_function` (func_def.c).  The specifiers of a
friend redeclared in a class definition are cached like those of its first
declaration; they are kept on the declaration's routine fixup
(`redecl_contract_specifiers`) and scanned, in that declaration's parameter
scope, and matched when the class is complete
(`defer_contract_redeclaration_match`,
`match_redeclared_contract_specifiers`).

Restrictions: a virtual function (beside the trailing-requires-clause
check), a deleted function and a function defaulted on its first
declaration (in `check_defaulted_or_deleted_function`) cannot have
specifiers, which are then dropped; specifiers anywhere but the declarator
of a function declaration are diagnosed and skipped
(`skip_function_contract_specifiers`).  A result name cannot be introduced
by a constructor or destructor, cannot have a parameter's name (other than
"_", a name-independent declaration) nor, on a lambda, the name of one of
its init-captures, which inhabit the lambda scope, and with a deduced
return type can appear only on a definition.  A declaration of `::handle_contract_violation`
is checked (`check_handle_contract_violation`): not inline or deleted, C++
language linkage, void return, one parameter of type
`const std::contracts::contract_violation &`.

The new functions are a block at the end of declarator.c; the calls to them
from the parsing code (the result-name checks, the specifier skip, the
attach sites in decls.c and class_decl.c) are in 1040-p2900-base-parse.

The tests are `tests/tests/contracts/sema/{redeclarations,restrictions,
handler,handler_ok}`.

## Compile gap

None.

## Contents

- src/declarator.c : #13, @difference:0, @difference:4, @difference:6
- src/declarator.h : #3
- src/class_decl.h : #2
- src/class_decl.c : /attach_contract_specifiers\(rp, state/, /redecl_contract_specifiers/, /ec_contract_on_deleted_func/, /ec_contract_on_virtual_func/
- src/func_def.c : /attach_contract_specifiers/
- src/il.c : #4, #5, #6, #9
- src/il.h : /equiv_contract_predicates|CC_AS_WRITTEN/
- src/error_msg.txt : /(^|;)ec_(contract_redecl_(?!message|captures|label|requires)\w+|contract_on_\w+|contract_result_name_(ctor|dtor|shadows_param|shadows_init_capture|deduced_nondef)|hcv_\w+);/
- src/Changes : @is:1, @the:0
- src/Changes : /^Contracts \(P2900\): redeclarations and restrictions$/
- src/Changes : /^A redeclaration of a function either omits the function contract$/
- src/Changes : /^specifiers of its first declaration or repeats them: the same number, in$/
- src/Changes : /^the same order, each of the same kind, with a result name on both or$/
- src/Changes : /^neither, and with the same predicate as written \(a parameter or a result$/
- src/Changes : /^name may be renamed\)\.  Otherwise it is an error, as is a redeclaration that$/
- src/Changes : /^                                          \/\/ condition in declaration$/
- src/Changes : /^deleted function, on a function defaulted on its first declaration, and$/
- src/Changes : /^anywhere other than the declarator of a function declaration \(a typedef, a$/
- src/Changes : /^type-id, a declaration of non-function type\)\.  A result name is diagnosed$/
- tests/tests/contracts/sema/*redeclaration* : *
- tests/tests/contracts/sema/*result_name_templated* : *
- tests/tests/contracts/sema/*restrictions* : *
- tests/tests/contracts/sema/*handler* : *
