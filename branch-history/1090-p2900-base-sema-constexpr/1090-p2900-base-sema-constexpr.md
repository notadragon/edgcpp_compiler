---
id: 1090-p2900-base-sema-constexpr
subject: 'contracts: P2900: evaluate contract assertions during constant evaluation []'
depends: [1040-p2900-base-parse]
regenerates: []
fixes: []
---

## Rationale

Evaluate contract assertions in the interpreter (interpret.c), from the
routines' contract specifiers and the `stmk_contract_assert` statements, so
that constant evaluation honors them for every back end.  A function's
preconditions are evaluated when its call starts -- for a constructor before
its mem-initializers -- and its postconditions when it returns successfully,
for a destructor after its members and bases are destroyed; a
`contract_assert` where it is reached.  In a predicate, a parameter
reference designates the callee's parameter variable, `this` the callee's
`this` parameter, and a postcondition's result name the call's result
location (`an_interpreter_state::contract_frame` and
`contract_result_name`).  A lambda in the predicate captures the parameter
variable a proxy stands for (each proxy is mapped onto it while its
specifier is evaluated, `with_param_proxies_mapped`) and the result itself
(`contract_result_location`).  The parameters live until the call
completes, so their storage is live while the postconditions are evaluated
(`params_alloc_seq_number`), for a reference or a pointer to one.

Under ignore nothing is evaluated.  Otherwise a predicate that is false or
not a core constant expression is recorded (`a_contract_problem`) and the
interpretation continues.  When an interpretation is complete
(`finish_contract_evaluation`, called by every entry point), the problems are
dropped if it failed on other grounds; make it fail, silently, if it is not
manifestly constant-evaluated (`std::is_constant_evaluated()` false), so that
the expression is evaluated at run time where the assertions are checked;
and otherwise are diagnosed at their assertions -- an error under enforce and
quick_enforce, which makes the value an error constant, a warning under
observe, where the value stands.  At most eight are listed per evaluation,
and a problem already reported for the same expression is not reported
again.  Problems met in an evaluation whose result is discarded (an
assumption's operand, `__builtin_constant_p`) are dropped.

A predicate's side effects do not persist: each complete object that existed
before the predicate began is saved before the predicate first modifies it
(the assignment operators, `__builtin_memcpy`, placement new and explicit
destructor calls, through `side_effect_allowed` and
`save_contract_snapshot`), the copies are put back once the predicate's
value is known, and the storage it allocated is freed.  That is an
evaluation without side effects that produces the predicate's value, which
P2900 permits.  Freeing storage allocated before the predicate began cannot
be undone this way, so it makes the predicate not constant.

The checks the front end generates for the C-generating back end are marked
(`a_statement::is_contract_check`, set by 1100-p2900-base-lowering) and
skipped, so that back end's constant evaluation follows the same rules; a
`contract_assert` whose predicate moved into its check is evaluated from the
check's condition, negated.

A reviewer should check the snapshot extent in `save_contract_snapshot`
(the object's value and the part of its prefix holding its flags and
initialization bitmap) and the places that call it.

The tests are in `tests/tests/contracts/constexpr`: each semantic, in
manifestly and not manifestly constant evaluation, the result name,
lambdas, templates, constructors and destructors, and side effects.

## Compile gap

None.

## Contents

- src/interpret.c : #3, #4, #5, #6, #7, #8, #10, #11, #12, #13, @any:0, @any:2, #15, #17:0, #17:2, #18, #19, #20, #21, #22, #23, #24, #25, #28, #32:1, #35, #36, #38, #39, #46, #49, #50, #51, #54, #55, #56, #57, #59, #60, #61, #62, #63, #64, #65, #66, #67, #68, #69, #70, #71, #74, #75, #76, #77, #78, #79, #80, #81, #82, #83, #84, #85, #86, #41, #45
- src/il_def.h : /is_contract_check/
- src/il_alloc.c : /is_contract_check/
- src/il_display.c : /is_contract_check/
- src/error_msg.txt : /(^|;)ec_(contract_predicate_false_in_constexpr|contract_predicate_not_constant|contract_one_more_problem_not_shown|contract_more_problems_not_shown|constexpr_contract_predicate_deallocation);/
- src/Changes : @the:1, @not:0
- src/Changes : /^Contracts \(P2900\): constant evaluation of contract assertions$/
- src/Changes : /^Contract assertions are now evaluated during constant evaluation, for every$/
- src/Changes : /^back end: a function's preconditions when a call starts \(for a constructor,$/
- src/Changes : /^before its mem-initializers\), its postconditions when it returns \(for a$/
- src/Changes : /^destructor, after its members and bases are destroyed\), and a$/
- src/Changes : /^                                \/\/ constant expression \(at the "pre"\)$/
- src/Changes : /^objects it modifies are restored once its value is known, as if an$/
- src/Changes : /^equivalent evaluation without side effects had been performed \(which P2900$/
- tests/tests/contracts/constexpr/* : *
- tests/expectations/edg_x86_64/contracts.log : /^constexpr\//
