---
id: 0140-constexpr-destroyed-object
subject: 'Reject destroying or writing to a destroyed object in constant evaluation []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A fix to upstream EDG, independent of contracts.  Ending the lifetime of an
object that is not within its lifetime, or storing to it, is undefined
behavior ([basic.life]), so not a core constant expression.  Constant
evaluation accepted a destructor call or a delete-expression after an
explicit destructor call (or the destruction of a local at the end of its
scope after one), and a store to the object: the interpreter's lifetime
state is only the initialization bits, and a destroyed object's storage is
merely uninitialized, which since C++20 may be written to.

A new complete-object flag, `COMPLETE_OBJ_DESTROYED`, is set when a whole
complete object is destroyed (by a destructor or a pseudo-destructor call)
and cleared when one is constructed there again (a constructor, or the
placement new of `std::construct_at`).  A destructor call, delete-expression,
pseudo-destructor call, assignment or assignment-operator call on such an
object fails ("attempt to destroy an object outside its lifetime", "attempt
to modify an object outside its lifetime").  Destroying a subobject or an
element of an array is not tracked.  The flag lives in the object's prefix,
so the contracts branch's snapshots of objects a predicate modifies restore
it.

Tracked in `bug-reports/` (EDG-44) until upstream fixes it.

## Compile gap

None.

## Contents

- src/interpret.c : #1, #9, #29, #34, #37, #40, #47, #48, #52, #53, #58
- src/error_msg.txt : /(^|;)ec_constexpr_(destroying|modifying)_outside_lifetime;/
- tests/tests/contracts/sema/*constexpr_object_lifetime* : *
- tests/expectations/edg_x86_64/contracts.log : /^sema\/constexpr_object_lifetime/
