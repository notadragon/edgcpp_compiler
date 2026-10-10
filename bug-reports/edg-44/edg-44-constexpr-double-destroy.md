# EDG-44: constant evaluation accepts destroying an object twice and writing to it after destruction

**Status:** Open upstream; fixed on this branch (band 0100,
`COMPLETE_OBJ_DESTROYED` in interpret.c).  Not contracts-specific.
**Component:** interpret.c: a destructor call and a delete-expression do not
check that the object is within its lifetime, and neither does a store
(a read after destruction is diagnosed: "access to uninitialized object").
**Upstream Link:** None found (searched 2026-10-01).
**Affects:** stock EDG at `90b66472e0` (`cpfe --c++26 --no_code_gen`) and
this branch.
**Found:** M3c import sweep, 2026-10-01: GCC's `constexpr-double-destroy`,
`constexpr-heap-double-destroy`.
**Test:** `tests/tests/contracts/sema/constexpr_object_lifetime.sft.cpp`.

## Bug Report

Constant evaluation accepts ending an object's lifetime twice -- an
explicit destructor call followed by a delete-expression, or by the
implicit destruction of a local -- and accepts a store to an object whose
lifetime has ended.  Each is undefined behavior ([class.dtor], [basic.life]),
so the evaluation is not a core constant expression ([expr.const]).

## Reproducer

[`constexpr-double-destroy.cpp`](constexpr-double-destroy.cpp):

    cpfe --c++26 --no_code_gen constexpr-double-destroy.cpp

reports nothing; all three static_asserts are accepted.  GCC ("destroying
an allocated object outside its lifetime", "modification of an allocated
object outside its lifetime is not a constant expression", "destroying 's'
outside its lifetime") and Clang ("static assertion expression is not an
integral constant expression") reject all three.

## Notes

Through edg-gxx on this branch g++ rejects the program, since the
static_asserts reach it in the generated code; the front end's own
evaluation is what is wrong.

## Fix (this branch)

The interpreter's only lifetime state was the initialization bits, and a
destroyed object's storage is merely uninitialized, which since C++20 may
be written to.  A complete-object flag, `COMPLETE_OBJ_DESTROYED`, is set
when a whole complete object is destroyed (destructor or pseudo-destructor
call) and cleared when one is constructed there again (a constructor, or
the placement new of `std::construct_at`); a destructor call,
delete-expression, pseudo-destructor call, assignment or assignment
operator call on such an object fails ("attempt to destroy an object
outside its lifetime", "attempt to modify an object outside its
lifetime").  Not covered: destroying a subobject (`s.m.~M()`) or an element
of a dynamically allocated array, which would need per-subobject state.
