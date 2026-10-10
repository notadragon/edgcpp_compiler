---
id: 1000-p2900-base-basic
subject: 'contracts: P2900: options, keywords and diagnostics []'
depends: []
regenerates: []
fixes: []
---

## Rationale

The configuration surface of the contracts facility, ahead of any parsing:
the options, the keyword and the feature-test macro.

Contracts are on by default in C++26 mode, as in GCC, and `--contracts` /
`--no_contracts` turn them on or off in any C++ mode; enabling them in C is
an error.  `__cpp_contracts` is defined whenever they are on -- also in g++
emulation below C++26, since g++ defines it whenever `-fcontracts` is in
effect.  `contract_assert` becomes a keyword only when contracts are on.

`--contract_evaluation_semantic=X` (ignore, observe, enforce, quick_enforce)
selects the semantic of every assertion; enforce is the default.  The
C-generating back end implements exception handling with setjmp/longjmp and
cannot propagate an exception out of a violation handler, so with it only
the semantics that can never let an exception escape a check are accepted:
ignore (the predicate is not evaluated) and quick_enforce (the handler is
never called), the latter being the default.  The option spellings are the
mechanical mapping of GCC's, so `edg-gxx` and the test importer can translate
GCC flags one for one.

In IFC module files the new keyword is written as a textual token (its
spelling, re-lexed on import), so the IFC format is unchanged.

A reviewer should check that the C-generating default does not make every
C++26 compilation through eccp an error: the restriction applies only to an
explicitly requested semantic.

## Compile gap

`contract_assert` is entered as a keyword but no statement parses it until
1040-p2900-base-parse, so a program using it is a syntax error here.  The
two result-name diagnostics belong to that commit.

## Contents

- src/cmd_line.c : #1:0, #2:1, #3, #4:0, #4:3, #4:6, #8:0
- src/cmd_line.h : #0:0, /^\t\tcontracts_enabled;$/, /^enum a_contract_evaluation_semantic \{$/, @a_contract_evaluation_semantic:3, /^\t\tcontract_evaluation_semantic;$/
- src/host_envir.h : *
- src/fe_init.c : /tok_contract_assert, "contract_assert"/
- src/macro.c : #0, #1:0, #1:2
- src/lexical.h : /tok_contract_assert/
- src/ifc_modules_write.c : /tok_contract_assert/
- src/il_def.h : /tok_contract_assert,/, /"contract_assert",/
- src/error_msg.txt : /(^|;)ec_cl_contracts_option_only_in_cplusplus;/
- util/eccp.sh : #0:1, #0:3, #1:0, #2:0, #3:0, #4:0
