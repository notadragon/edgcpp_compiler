# EDG-111: a requires-clause on a member typedef of function type in a class template is accepted

**Status:** Open upstream and on this branch.  Not contracts-specific.
**Component:** the parsing of a member-declarator's requires-clause
(decl_spec.c / declarator.c), for a typedef.
**Upstream Link:** None found (searched 2026-10-09).
**Affects:** stock EDG at `0ac366374c` (cpfe and cpfe-cp), C++20 and later.
**Found:** M12-EDG, 2026-10-09, mirroring GCC's
`g++.dg/cpp2a/concepts-requires-declarator.C` (EDG-101).
**Watch test:** `tests/tests/contracts/openbugs/edg-111-requires-clause-member-typedef.sft.cpp`.

## Bug Report

[dcl.decl.general]: the optional requires-clause of an init-declarator or
member-declarator shall be present only if the declarator declares a
templated function.  A member typedef declares a type, yet

    template <class T> concept C = sizeof(T) > 0;
    template <class T> struct A {
      typedef int F(int) requires C<T>;
    };
    A<int> a;

is accepted.  The same typedef at namespace scope is rejected ("typedef may
not be specified here" for the template, "a requires-clause is not allowed
here (not a templated function)" for a plain one), as are the alias
declaration, the data member and the parameter of the same test.

## Reproducer

    cpfe --c++20 --no_code_gen t.cpp     # no diagnostic
