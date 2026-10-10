---
id: 0190-nested-this-capture-of-copy
subject: 'Fix capturing this from an enclosing lambda''s copy of *this where this has no variable []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A fix to upstream EDG, independent of contracts.  Where `this` has no
variable -- a default member initializer, and on this branch the predicate
of a function declaration's precondition or postcondition -- a lambda that
captures `this` from an enclosing lambda that captured `*this` by copy
stopped the front end ("type_pointed_to: not a pointer type").  The
capture's member took the enclosing closure's member type, the copy itself,
and its initialization copied the copy.  As for a capture with a `this`
variable, the member is now a pointer to the enclosing lambda's copy (and a
capture of `*this` copies the object, whichever the enclosing member is),
and the initialization takes the copy's address, or copies the object an
enclosing capture of `this` points to (`make_field_for_lambda_capture`, the
closure initialization in expr.c, its lowering in lower_init.c and its
constant evaluation in interpret.c).  An implicit capture of `this` through
an enclosing lambda's capture is by reference, as it is where `this` has a
variable (`r_add_lambda_capture`).

Tracked in `bug-reports/` (EDG-81) until upstream fixes it.

## Compile gap

None.

## Contents

- src/class_decl.c : #11, #12, #16, #17
- src/expr.c : #53, #54
- src/interpret.c : #42, #43, #44
- src/lower_init.c : #0, #1
- src/overload.c : #6
- tests/tests/contracts/sema/*nested_this_capture_copy* : *
