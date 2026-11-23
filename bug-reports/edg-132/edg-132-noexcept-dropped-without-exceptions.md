# EDG-132: with exceptions disabled, the C++-generating back end drops a function's exception specification

**Status:** Open upstream; open (defect) on this branch.
**Component:** declarator.c `scan_exception_specification` and the
routine-type copy beside it ("When exceptions are disabled, no exception
specification should be recorded"); cp_gen_be.c then has nothing to put out.
**Upstream Link:** None found (searched 2026-10-11).
**Affects:** stock EDG at `0ac366374c` with `--no_exceptions` (g++'s
`-fno-exceptions`) whenever its generated C++ is compiled by another compiler
(the C++-generating back end, `cpfe-cp`; here through `util/edg_gxx.sh`).
**Found:** M44-EDG, 2026-10-11: Clang's `Runnable/noexceptions-contracts`,
once run (`imported_p3098/clang`): `noexcept(non_throwing(1))` is false at
run time for a function declared `noexcept`.
**Test:** `tests/tests/contracts/openbugs/edg-132-noexcept-dropped-without-exceptions.sft.cpp`
records the current behavior (both errors, from g++).

## Bug Report

With exceptions disabled the front end records no exception specification
on a function declaration, so the C++ that the C++-generating back end puts
out declares the function without its `noexcept`.  The `noexcept` of a
function pointer type is kept, and a `noexcept(...)` operator is put out as
written, so the compiler of the generated code sees a different program:
a valid conversion from `&g` to a `noexcept` function pointer is rejected,
and `noexcept(g())` is false.  Compiled directly, g++ (and Clang) accept the
program with `-fno-exceptions`: the exception specification stays part of
the function's type (C++17 and later).

## Reproducer

[`noexcept-dropped-without-exceptions.cpp`](noexcept-dropped-without-exceptions.cpp),
with the stock `cpfe-cp` and the GCC paired with it:

    cpfe-cp --gnu_version=170000 --c++26 --no_exceptions --gen_c_file_name=gen.cpp noexcept-dropped-without-exceptions.cpp
    g++ -std=c++26 -fno-exceptions -c gen.cpp

(the front end needs `EDG_BASE` and g++'s include directories as
`--sys_include=`; measured through our `util/edg_gxx.sh`, which passes them,
with `CPFE_CP` set to a stock build: g++ reports "invalid conversion from
'void (*)()' to 'void (*)() noexcept'" and "static assertion failed").  The
generated declaration is `void g() { }`.

## Notes

The front end itself is consistent: without exceptions it answers every
noexcept question with true (EDG-70, deferred), so it never needs the
specification.  The generated code is compiled by a compiler that still
reads exception specifications, which is why the specification has to
survive there.  The C-generating back end is unaffected (no exception
specifications in C).  Not contracts-specific.
