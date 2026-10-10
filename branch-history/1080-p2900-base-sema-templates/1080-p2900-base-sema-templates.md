---
id: 1080-p2900-base-sema-templates
subject: 'contracts: P2900: instantiate the contract assertions of templates []'
depends: [1040-p2900-base-parse]
regenerates: []
fixes: []
---

## Rationale

Give each instance of a templated function its own function contract
specifiers.  1040-p2900-base-parse scans a template's predicates in the
template and keeps their tokens; this commit scans them again for an
instance, from those tokens, in a template instantiation context: a
function prototype scope recreated as for the instantiation of an exception
specification (`instantiate_contract_specifiers_if_needed`, modeled on
`instantiate_exception_spec_if_needed_full`).  What was parsed in the
template is then substituted rather than parsed again -- a
requires-expression in particular, which EDG records by token position
(`requires_ranges`), so an invalid requirement makes it false instead of
being an error.

An instance's specifiers are instantiated when the instance is odr-used
(`mark_routine_referenced_full`, next to the exception specification) or
its definition is instantiated (`instantiate_template_function_full`,
before the body is scanned), and not when it is only named in an
unevaluated operand -- the behavior of GCC and Clang that their tests pin.
A predicate may odr-use the function itself, or a function whose predicate
odr-uses this one, so an instance whose specifiers are being instantiated
is left alone (`contract_instantiations`, a stack).
A member function template's predicates are scanned in the template when
its class is complete (`scan_member_template_contract_specifiers`, from the
second pass of the class's routine fixups); that is what lets the
instantiation substitute their requires-expressions.

This covers function templates (including one declared before it is
defined), members of class templates, member templates and generic lambdas.
The C++-generating back end prints templates from their tokens, so in the
edg-gxx pairing it is g++ that instantiates them; EDG's instances reach the
C-generating back end.

The tests are in `tests/tests/contracts/templates`: when a template's
predicates are scanned and diagnosed (in the template, on odr-use, not in
an unevaluated operand) and requires-expressions in the predicates of every
kind of templated function.

## Compile gap

None.

## Contents

- src/templates.c : #0, #1, #10, #4, #5, #6, #7, #8, #9, #11, #12
- src/symbol_tbl.c : #1, #4
- src/symbol_tbl.h : #0, #1, #7
- src/decls.c : #2
- src/class_decl.c : #7, #8, #9, #26
- src/class_decl.h : #1
- src/expr.c : #35, #36, #37, #38
- src/il.c : #2, #7, #8
- src/il_alloc.c : #3
- src/il_def.h : #11
- src/il_display.c : #4
- src/templates.h : /instantiate_contract_specifiers_if_needed/
- src/il.c : /instantiate_contract_specifiers_if_needed/
- src/Changes : @not:1, @leaves:0
- src/Changes : /^Contracts \(P2900\): contract assertions of templates$/
- src/Changes : /^The contract assertions of function templates, of members of class$/
- src/Changes : /^templates, of member templates and of generic lambdas are scanned in the$/
- src/Changes : /^template, and each instance gets its own, scanned again from the template's$/
- src/Changes : /^tokens when the instance is odr-used or its definition is instantiated --$/
- tests/tests/contracts/sema/*explicit_specialization* : *
- tests/expectations/edg_x86_64_gxx/contracts.log : /^sema\/explicit_specialization_in_class/
- tests/tests/contracts/templates/* : *
