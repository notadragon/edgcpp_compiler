# EDG-45: constant evaluation accepts referring to a member before its constructor begins

**Status:** Open upstream; fixed on this branch (band 0100,
`member_of_unconstructed_object` in interpret.c).  Not contracts-specific.
**Component:** interpret.c: forming the address of a subobject of a member
whose construction has not begun is not checked.
**Upstream Link:** None found (searched 2026-10-01).
**Affects:** stock EDG at `90b66472e0` (`cpfe --c++26 --no_code_gen`) and
this branch.
**Found:** M3c import sweep, 2026-10-01: GCC's
`open-bug-address-before-ctor` (the `direct_member` case, xfailed there;
the importer now skips tests with an xfail `dg-error`).
**Test:** `tests/tests/contracts/sema/constexpr_member_before_construction.sft.cpp`.

## Bug Report

For an object with a non-trivial constructor, referring to a non-static
member before the constructor begins is undefined behavior
([class.cdtor]/1), so an evaluation that does it is not a core constant
expression ([expr.const]).  EDG accepts a constructor whose mem-initializer
takes the address of a member of a later member.

## Reproducer

[`member-address-before-ctor.cpp`](member-address-before-ctor.cpp):

    cpfe --c++26 --no_code_gen member-address-before-ctor.cpp

reports nothing; the static_assert is accepted.  Our GCC and Clang accept
it too (GCC tracks it with GCC-2, whose virtual-base case Clang gets
right).

## Notes

The virtual-base case of the same GCC test is rejected by EDG, for another
reason (a class with a virtual base has no constexpr constructor).

## Fix (this branch)

There is no construction-has-begun state, but the work stack holds it: a
constructor interpreting its mem-initializers has the ones not yet started
in its payload.  A member selection (`.`, `->`, `.*`, `->*`) from an object
within a complete object that is not yet fully initialized looks for such a
constructor of that complete object and fails if the object lies in a
member the constructor is still to initialize with a non-trivial
constructor ("attempt to refer to a member of an object whose construction
has not begun").  Forming the member's own address stays valid
([class.cdtor]/3).  Not covered: base classes reached early, member calls
on an unconstructed member, members of aggregates being initialized,
variant members.
