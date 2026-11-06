# EDG-159: header units and `import std;` -- the D4451 using-directive survey cannot be run in GNU mode

**Status:** Open here (Kind: defect until the user decides otherwise).
**Upstream Link:** -- (the underlying gaps are EDG's GNU-mode module
support, recorded with contracts in EDG-54).
**Affects:** measured 2026-11-03 against this branch's install
(`d61029c446`) and its cpfe-cp in GNU mode with the gnu_gcc fork's
libstdc++ (`g++ 17.0.0 20261029`).
**Watch test:** none.  edgy can run header-unit tests
(`//header_unit_files:`), but natively every `modules/eifc/header-unit`
test is a CATASTROPHE deviation (2026-10-02 baseline), and the contracts
suite's configurations go through edg-gxx (which does not support
modules) or eccp; the same reason EDG-54 has none.

## What cpfe supports

* **edg-gxx: no modules at all.**  `-fmodules` and `-fmodule-header` go to
  g++ only, and a header (`-x c++-header`, `.h`) is not a source to it, so
  it goes to g++ untouched (g++ alone builds its CMI).  cpfe-cp never gets
  `--modules`, so `import` is parsed as an identifier ("this declaration has no storage class or type
  specifier", "import is not a template").  With `-Xedg --modules`, an
  `import "h.h";` is "not an importable header" (no mapping), and
  `import std;` "could not find module file for module "std"".  Even with a
  header unit, cpfe-cp's generated C++ keeps `import "h.h" ;`, which g++
  would need its own CMI for, and edg-gxx has no step to build one.
* **cpfe-cp by hand, user header units: yes, in GNU mode.**  With edg-gxx's
  own front-end options (`--gnu_version=170000`, g++'s include list) plus
  `--c++26 --modules --no_code_gen`, `--create_header_unit=H.eifc H.h`
  writes a header unit for a small user header and `--header_unit=H.h=H.eifc`
  imports it (stock EDG's GNU-mode module failure, EDG-54, is for
  `--module_interface`).
* **libstdc++ header units: no.**  `--create_header_unit` of the fork's
  `<chrono>`, or of `<contracts>` under `--contracts_p3400` or
  `--contracts_p4451`, is "Catastrophic error: an IFC file could not be
  produced for the current translation unit / one or more entities cannot
  currently be written to an IFC file" -- the same failure as EDG-54's
  GNU-mode reproducer.
* **`import std;` / `import std.compat;`: no.**  "could not find module file
  for module "std"" (and "std.compat"), in GNU mode and without it.  EDG
  reads a standard library module only as an MSVC IFC file (Microsoft
  mode); it cannot build one from libstdc++, whose entities it cannot write
  to an IFC file (above).

## The 8 cases (notadragon_wg21 `p4451-milestones.md`)

| # | Case | D3400R6 rule | EDG |
|---|---|---|---|
| 1 | header unit, global `using namespace N;`, name used unqualified by an importer | [module.interface]: every using-directive in a header unit is exported | **passes** (cpfe-cp by hand only) |
| 2 | same, directive nested in a namespace | same | **passes** (cpfe-cp by hand only) |
| 3 | `import <chrono>;`, `using namespace std::chrono; 1h` | [std.modules]: an importable library header's specified using-directives are exported | cannot run: no `<chrono>` header unit |
| 4 | `import std;`, the same | [std.modules]: `std` exports the specified using-directives of the importable library headers | cannot run: no `std` module |
| 5 | `import std.compat;`, the same | same | cannot run: no `std.compat` module |
| 6 | `import <contracts>;`, `pre<terminating>` and another P3400 label, P3400 and P4451 | [module.interface] with `<contracts>`'s `contract_control` directive | cannot run: no `<contracts>` header unit |
| 7 | `import std;`, the same labels | [std.modules] | cannot run: no `std` module |
| 8 | a header unit's `contract_control` directive is inactive outside assertion-control scopes | the `contract_control` directive's scope rule, through [module.interface] | (a) user header unit: **fails** (EDG-54); (b) `<contracts>`, (c) `import std;`: cannot run, as 6 and 7 |

Cases 1 and 2: [`edg-159-hu-directives.h`](edg-159-hu-directives.h) and
[`.cpp`](edg-159-hu-directives.cpp) (GCC's `using-directive-hu-1_{a,b}`
with the variables made functions) give exactly the one expected error, at
`g4`, as the same file does with the header `#include`d.  Upstream's
`modules/eifc/header-unit/using-directive{,-namespace}` cover the same, in
non-GNU mode.

Case 8(a): [`edg-159-hu-contract-control.h`](edg-159-hu-contract-control.h)
and [`.cpp`](edg-159-hu-contract-control.cpp).  Through the header unit the
directive becomes an ordinary using-directive: `leak1` and `leak2` are
accepted (`#include`d, both are errors, as expected).  This is EDG-54's
"the `contract_control` flag of a using-directive, which IFC files do not
carry", so it has no row of its own.

    O="--c++26 --modules --no_code_gen --gnu_version=170000 --contracts_p3400"
    cpfe-cp $O --create_header_unit=H.eifc H.h
    cpfe-cp $O --header_unit=H.h=H.eifc T.cpp

(with edg-gxx's `--sys_include=` list and `EDG_BASE` set to the install's
`base/`; `EDG_GXX_VERBOSE=1 edg-gxx -fsyntax-only` prints them).

## Also found: no variable reaches an importer through a header unit

Running GCC's case-1 test as written failed for an unrelated reason: a
variable declared in a header unit -- `inline int v = 1;` or
`extern const T v;`, at namespace scope -- is not found by the importer
even by qualified name ("namespace "hu1_n" has no member "hu1_v""), in GNU
mode and without it.  Functions, types and using-directives are found.
Not contracts-specific; provenance not established on a stock build (the
branch's only change to the IFC code is four contract token kinds in
`ifc_modules_write.c`'s token serialization).  The reproducers above use
functions to stay clear of it.

## Mirrors

CLANG-882's program (`import std;` / `import std.compat;` with
`std::chrono` literals) and cases 8(b)/(c): N/A, not runnable (no `std`
module or libstdc++ header unit).  Cases 1, 2 and 8(a): runnable only by
hand through cpfe-cp, not in edgy (see "Watch test"); the reproducers here
are their record.
