---
id: 6910-p4451-callerside-later
subject: 'contracts: P4451: caller-side preconditions after the non-checking semantics []'
depends: [1500-p4451, 2400-p3595, 4285-p4451-under-p3400, 4800-p4298, 4900-p4405, 6000-p3100-core, 6900-cross-paper-options]
regenerates: []
fixes: []
---

## Rationale

P4451's "Caller-Side Evaluations" with the semantics of the other papers
(notadragon_wg21 DECISIONS.md E181 (2)), as the gnu_gcc fork's
`p4451-callerside-later`: a precondition checked at the call makes an
earlier terminating one checked there too, and a later precondition counts
unless its semantic at the call has the run-time effect of ignore.  So
P4405's prove and diagnose do not count, while P3100's assume, P4298's
noexcept semantics and a dynamically selected semantic do.  g++ makes the
checks (through edg-gxx).  Run under `--contracts_p4451` alone,
`--contracts_p3400` alone (`-p3400`) and both (`-both`), with a P3595
configuration.  It sits after the last paper it needs, P3100's
`--contracts_allow_assume`.

## Compile gap

None.  Pure test additions.

## Contents

- tests/tests/contracts/imported_p4451/gcc/p4451-callerside-later/* : *
- tests/tests/contracts/imported_p4451/gcc/p4451-callerside-later-p3400/* : *
- tests/tests/contracts/imported_p4451/gcc/p4451-callerside-later-both/* : *
- tests/expectations/edg_x86_64/contracts.log : /^(imported_p4451\/gcc\/p4451\-callerside\-later\/test\.mft\.cpp|imported_p4451\/gcc\/p4451\-callerside\-later\-p3400\/test\.mft\.cpp|imported_p4451\/gcc\/p4451\-callerside\-later\-both\/test\.mft\.cpp):/
