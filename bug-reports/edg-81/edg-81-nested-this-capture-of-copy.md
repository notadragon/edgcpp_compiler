# EDG-81: capturing "this" from an enclosing lambda's copy of *this in a default member initializer is an internal error

**Status:** Open upstream; fixed on this branch (band 0100, class_decl.c
`make_field_for_lambda_capture`, expr.c the closure initialization).  Not
contracts-specific.
**Component:** class_decl.c (`make_field_for_lambda_capture`,
`r_add_lambda_capture`), expr.c (the initialization of a closure object's
captures), lower_init.c and interpret.c (its lowering and constant
evaluation).
**Upstream Link:** None found (searched 2026-10-04).
**Affects:** stock EDG at `0ac366374c`, in any C++ mode with lambdas and
`*this` captures.
**Found:** M10c, 2026-10-04, by a GCC test of contract predicates
(`contract-predicate-constify-lambda`); the predicate of a declaration, like
a default member initializer, has no `this` variable.
**Test:** `tests/tests/contracts/sema/nested_this_capture_copy.sft.cpp`.

## Bug Report

In a context with no `this` variable (a default member initializer), a
lambda that captures `this` from an enclosing lambda that captured `*this`
by copy stops the front end:

    internal error: type_pointed_to: not a pointer type

The capture's member is given the enclosing closure's member type (the
copy, not a pointer to it), and its initialization copies the copy instead
of taking its address.  In a member function, where `this` is a variable,
the same program is accepted.  GCC and Clang accept both.

## Reproducer

[`nested-this-capture.cpp`](nested-this-capture.cpp):

    cpfe-cp --c++20 --no_code_gen nested-this-capture.cpp

## Fix (this branch)

As for a capture of `this` that has a variable: the member is a pointer to
the enclosing lambda's copy (and a `*this` capture copies the object it
points to), and the initialization takes the copy's address.  The reverse
shape, a `*this` capture from an enclosing lambda's capture of `this`, had
the pointer's type for its member; with the object's type, the C-generating
lowering and the constant evaluation of its initialization now copy the
object the enclosing member points to.  An implicit capture of `this`
through an enclosing lambda's capture is by reference.
