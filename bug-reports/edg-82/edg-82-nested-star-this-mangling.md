# EDG-82: C-generating back end: a lambda capturing *this nested in another in a default member initializer is an internal error when local types are mangled

**Status:** Open upstream and on this branch; deferred by the user
2026-10-04.  Not contracts-specific.
**Component:** il.c (`enclosing_routine_for_local_type`), lower_name.c (the
mangling of local type names at file-scope wrap-up).
**Upstream Link:** None found (searched 2026-10-04).
**Affects:** stock EDG at `0ac366374c` with the C-generating back end
(`cpfe`), C++17 and later.
**Found:** M11a, 2026-10-04, extending the test of EDG-81.
**Watch test:** `tests/tests/contracts/openbugs/edg-82-nested-star-this-mangling.sft.cpp`.

## Bug Report

A default member initializer whose lambda captures `*this` and contains
another lambda capturing `*this`, in a class with another default member
initializer that has a lambda, stops the C-generating front end while the
names of local types are mangled:

    internal error: assertion failed at: "il.c", line 11384 in
              enclosing_routine_for_local_type

Without the other member's lambda, or without the inner `*this` capture, the
program compiles.  The C++-generating front end (and g++) accept it.

## Reproducer

[`nested-star-this-mangling.cpp`](nested-star-this-mangling.cpp):

    cpfe --c++20 --gen_c_file_name out.c nested-star-this-mangling.cpp
