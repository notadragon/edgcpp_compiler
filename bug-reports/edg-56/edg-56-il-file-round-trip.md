# EDG-56: an IL file cannot be read back when the translation unit has an attribute, an inline variable, and other common constructs

**Status:** Open upstream; not fixed on this branch.  Not
contracts-specific.  No shipped configuration writes IL files (only `cdisp`
reads them), so nothing on this branch depends on it.
**Component:** il_write.c / il_read.c (the IL file), il_display.c
(`disp_ptr_value`).
**Upstream Link:** None found (searched 2026-10-02); possibly related
[#30](https://github.com/edgcpp/compiler/issues/30) (ABORT in
`read_memory_region`, reflection).
**Affects:** stock EDG at `90b66472e0` and this branch, built with an
IL-file configuration (below).
**Found:** M5, 2026-10-02, round-tripping the contracts suite through IL
files.
**Watch test:** none: no edgy configuration writes IL files.

## Bug Report

A front end built to write its IL to a file (`IL_SHOULD_BE_WRITTEN_TO_FILE`)
writes files that `cdisp` cannot display for many ordinary translation
units: it stops with

    Internal error: assertion failed at: "il_display.c", line 210 in
              disp_ptr_value

(a pointer that is not inside the entries of its kind), and a Debug `cdisp`
also reports "read_memory_region: not all expected entries were read".
Each of these is enough, compiled as C++20:

* any attribute: [`attribute.cpp`](attribute.cpp) (`[[maybe_unused]] int g;`;
  also `[[deprecated]]`, an attribute on a parameter)
* an inline variable: [`inline-variable.cpp`](inline-variable.cpp) (also a
  `static constexpr` data member that is used)
* a local of a class type with a member whose destructor is not trivial,
  in a conditional block: [`member-destructor.cpp`](member-destructor.cpp)
* a variable template beside requires-clauses:
  [`requires-clause.cpp`](requires-clause.cpp)

Two smaller problems seen at the same time: `--il_display` together with
`-o file` writes a file `cdisp` rejects ("invalid intermediate language
file"), even for `int g(int x) { return x + 1; }`; and `cdisp` prints
function types without their `noexcept` (a destructor's `void (S *)
noexcept` is `void (S *)`), where the in-memory display has it.

## Reproducer

Build with the default configuration's `cpfe.cmakedef` replaced by

    IL_SHOULD_BE_WRITTEN_TO_FILE=1
    NEED_IL_DISPLAY=1

(e.g. `BUILD_TYPE=Release EDG_SRC=<clone> EDG_BUILD=<dir>
bin/edg/build.sh` in notadragon_wg21), then

    cpfe --c++20 -o t.il attribute.cpp
    cdisp t.il

## Notes

The contracts suite (421 tests, M5) was compared this way: `cpfe --il_display`
against `cdisp` of the file written by the same front end, addresses and
entry numbers normalized.  Every difference and every failure traced to one
of the constructs above or the missing `noexcept`; contract specifiers,
result names and `contract_assert` round-trip unchanged.
