---
id: 0150-constexpr-member-before-construction
subject: 'Reject referring to a member of an unconstructed member in constant evaluation []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A fix to upstream EDG, independent of contracts.  For an object with a
non-trivial constructor, referring to a non-static member before the
constructor begins is undefined behavior ([class.cdtor]/1), so not a core
constant expression.  Constant evaluation accepted a mem-initializer that
selected a member of a later member, as in `p(&z.k)` before `z` is
constructed.

A member selection (`.`, `->`, `.*`, `->*`) from an object within a complete
object that is not yet fully initialized now calls
`member_of_unconstructed_object`, which looks on the work stack for a
constructor interpreting its mem-initializers for that complete object
(phase `iwp_1st_resume`) and fails if the object lies in a member that the
constructor is still to initialize with a non-trivial constructor ("attempt
to refer to a member of an object whose construction has not begun").
Forming the address of the member itself stays valid ([class.cdtor]/3).
Not detected: base classes, variant members, and members of aggregates.

Tracked in `bug-reports/` (EDG-45) until upstream fixes it.

## Compile gap

None.

## Contents

- src/interpret.c : #33, #72, #73
- src/error_msg.txt : /(^|;)ec_constexpr_member_before_construction;/
- tests/tests/contracts/sema/*constexpr_member_before_construction* : *
- tests/expectations/edg_x86_64/contracts.log : /^sema\/constexpr_member_before_construction/
