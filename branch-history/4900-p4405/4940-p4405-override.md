---
id: 4940-p4405-override
subject: 'contracts: P4405: override obligations of virtual functions []'
depends: [4900-p4405]
regenerates: []
fixes: []
---

## Rationale

D4405R0's "Virtual Functions" as scaffolding (VF-EDG): a call through a
base class evaluates the contract assertions of the function it names,
B::f, and of the final overrider D::f, so for each function B::f that a
virtual function D::f overrides, directly or indirectly, D::f's
preconditions and B::f's postconditions are obligations of D::f -- in our
GCC, proven in an override shadow of D::f's body that takes B::f's
preconditions as given, a proving strategy of its own
(`-fcontracts-prove-override`).

`--[no_]contracts_prove_override` is that strategy's option, implied by
`--contracts_p4405` (an error in C, like the other contract options); the
drivers map `-f[no-]contracts-prove-override` to it and pass it on to g++,
which proves (or reports) every override obligation through edg_gxx.sh.
The front end proves nothing, so it does not consult the option: with
either setting it reports what our GCC reports without override shadows
(at `-O0`, or with `-fno-contracts-prove-override`).

With the C-generating back end, a virtual function's definition (a
compiler-generated destructor's too) records, per function it overrides --
found on the base classes' lists of overriding virtual functions; a base
class that occurs as more than one subobject gives one per subobject only
when the overridden function's contract assertions name `this` -- an
obligation for each of its own preconditions and each postcondition of the
overridden function whose analysis evaluation is prove or diagnose, but a
predicate folded to true (`note_contract_override_obligations`).  An
overridden function without contract assertions gives no premise, so the
overrider's preconditions are obligations all the same.  At the end of the
translation unit, once the needed flags are set, each obligation of a
definition that is put out is diagnosed at that definition, naming the
overridden function (by name, since the types are lowered by then): an
error for prove, a warning for diagnose (`contract_override_unproven`),
"contract assertion cannot be proven for a call through", with the
predicate's position (`diagnose_contract_override_obligations`, from
`file_scope_il_wrapup_part_3`).  An inline overrider whose class's virtual
function table is defined elsewhere is not put out, so not diagnosed.
After another error the needed flags are not set, and the obligations are
diagnosed for the definitions that would have been put out
(`contract_overrider_is_put_out`), as our GCC does since GCC-947: one that
is not inline; an inline one, or a member of a class template instance, if
its class's virtual function table is defined here (its decider function
defined here, or, with none, its virtual functions required by a
constructor or destructor defined here).  A predicate folded to true
settles an obligation unless `--no_contracts_prove_fold` (4900-p4405's).

The tests are `tests/tests/contracts/p4405override/` and GCC's
`p4405-override-*` imports in `tests/tests/contracts/imported_p4405override/`
(their dump scans dropped).

## Compile gap

None: the commit adds its own functions and option.  Intermediate commits
of the generated branch need not build (DECISIONS.md E2, as for GCC and
Clang).

## Contents
- src/Changes : /^With the C-generating back end, override obligations are diagnosed after$/
- src/Changes : /^10\/28\/26 \[\]$/, @definition:0
- src/Changes : /^Contracts \(P4405\): override obligations of virtual functions$/
- src/Changes : /^The new --\[no_\]contracts_prove_override option/
- src/Changes : /^is accepted for our GCC's override proving strategy, to which the drivers$/
- src/Changes : /^pass it on; the front end proves nothing, so it changes nothing there\.$/
- src/Changes : /^With the C-generating back end, a virtual function whose definition is put$/
- src/Changes : /^out has, for each function it overrides, directly or indirectly, an$/
- src/cmd_line.c : #1:14, #2:13, #4:22, #6:12, #8:13
- src/cmd_line.h : #0:14, /^\t\tcontracts_prove_override_enabled;$/
- src/error_msg.txt : /(^|;)ec_contract_override_(unproven(_warning)?|predicate);/
- util/eccp.sh : #0:18, #1:14, #2:13
- util/edg_gxx.sh : #0:15
- dev_tools/bin/eccp-gxx : #0:15
- src/statements.c : #1, #16
- src/statements.h : #2
- src/func_def.c : #5, #7
- src/fe_wrapup.c : #0, #1
- tests/tests/contracts/p4405override/* : *
- tests/tests/contracts/imported_p4405override/* : *
- tests/expectations/edg_x86_64/contracts.log : /^(p4405override|imported_p4405override)\//
- tests/expectations/edg_x86_64_gxx/contracts.log : /^(p4405override|imported_p4405override)\//
