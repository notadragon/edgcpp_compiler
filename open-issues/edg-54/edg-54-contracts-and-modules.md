# EDG-54: contract assertions are lost when a declaration is imported from a module

**Status:** Deferred by the user 2026-10-02 (notadragon_wg21 DECISIONS.md
E13): EDG's module support has many known upstream gaps, and contracts
across modules wait until it is usable.  EDG-25 is deferred with it.
**Component:** the IFC writer and reader (`ifc_modules_write.c`,
`ifc_modules_read.c`); for templates and inline definitions, the token
caches they store (`copy_template_body_to_cache`,
`get_function_definition_for_module_write`).
**Found:** M5 planning, 2026-10-02, at `25bd8becd7` (probes below).
**Watch test:** none.  Module tests cannot run under edgy's native
configurations, which use GNU mode (see "Modules in GNU mode"); every
`modules/eifc` test already deviates in the native baseline.

## Reproducer

Non-GNU mode, where EDG can write and read its IFC files:

    // m.ixx
    export module m;
    export constexpr int cf(int x) pre(x > 0) { return x; }
    export template<class T> constexpr T ct(T x) pre(x > 0) { return x; }
    export int nf(int x) pre(x > 1);
    export struct S { constexpr int mf(int x) const pre(x > 0) { return x; } };

    // t.cpp
    import m;
    constexpr int a = cf(0);       // predicate false
    constexpr int b = ct(0);       // predicate false
    constexpr int c = S{}.mf(0);   // predicate false
    int nf(int x) pre(x > 2);      // mismatched contract condition

    O="--c++26 --modules --contract_evaluation_semantic=enforce --no_code_gen"
    cpfe-cp $O --module_interface m.ixx
    cpfe-cp $O t.cpp

`t.cpp` compiles with no error (only "declared but never referenced"
warnings).  The same declarations in one translation unit give all four
errors ("contract predicate is false in constant expression" three times,
"mismatched contract condition in declaration").  So the IFC carries no
contract specifiers: not for non-template functions, members or
declarations, and not even for the template, whose stored tokens are
re-parsed on import.

## What does work

* **cpfe-cp output.**  The importing TU's generated C++ keeps `import foo;`,
  and the interface's generated C++ keeps `pre`/`post`/`contract_assert` as
  written (non-template, inline, template, declaration-only), so in the
  edg-gxx pairing the run-time checks would be g++'s once g++ builds the
  module from that output (`-fmodules`; edg-gxx has no support for that).
  What EDG's IFC is missing matters for EDG's own work in the importer:
  redeclaration matching, constant evaluation, the C-generating back end's
  checks of imported inline functions, and EDG-25's check.
* **Precompiled headers** keep contract assertions (Release, ASLR off --
  `setarch $(uname -m) -R`; with ASLR on, `--use_pch` reports a memory usage
  conflict and silently parses the header instead): the same three
  constant-evaluation errors and the mismatch are diagnosed through
  `--use_pch`.  PCH tests are in the contracts suite (M5).

## Modules in GNU mode (stock)

Upstream at the same revision, without contracts:

    // foo.ixx
    export module foo;
    export int bar(int x) { return x + 1; }

    cpfe-cp --c++20 --gnu_version=170000 --modules --module_interface --no_code_gen foo.ixx

    Catastrophic error: an IFC file could not be produced for the current
              translation unit
                one or more entities cannot currently be written to an IFC file

Without `--gnu_version` it succeeds.  Importing an EDG-written `.eifc` in GNU
mode is a catastrophic error too ("Microsoft mode must be enabled to use the
module file ... (a Microsoft Visual Studio IFC module)").  edg-gxx always
runs GNU mode, so it cannot compile a module at all, and edgy's native
configurations fail every `modules/eifc` test, EDG's own one-function
`modules/import-function` included (2026-09-30 baseline: 47 module test
deviations under `edg_x86_64`).  Not separately filed in `bug-reports/`:
upstream tracks EDG IFC's unwritten entity kinds as open issues -- among them
[#58](https://github.com/edgcpp/compiler/issues/58) (variables),
[#59](https://github.com/edgcpp/compiler/issues/59) (macros),
[#60](https://github.com/edgcpp/compiler/issues/60)/[#61](https://github.com/edgcpp/compiler/issues/61)
(conversion and operator functions),
[#70](https://github.com/edgcpp/compiler/issues/70) (alias templates),
[#73](https://github.com/edgcpp/compiler/issues/73)/[#74](https://github.com/edgcpp/compiler/issues/74)
(non-exported and global-module-fragment entities),
[#75](https://github.com/edgcpp/compiler/issues/75) (friends, explicit
specializations) -- and which one GNU mode trips was not investigated.

## Notes for when this is picked up

* The design proposed at M5 planning, not decided: an EDG-variant IFC trait
  (like `an_ifc_edg_trait_function_definition`) holding a declaration's
  contract specifier sequence as a token cache, re-scanned on import in the
  prototype scope the way member contracts are scanned in complete-class
  context (M2).  The alternative, IFC expression trees for the predicates,
  is far more code.  Microsoft's IFC variant has no place for contracts.
* Check the template token cache: it apparently stops short of the
  declarator's specifiers, or the re-parse drops them.
* `contract_assert` in an exported inline body: `ifc_modules_write.c` maps
  `tok_contract_assert` to `ifc_ebts_complex`; untested.
* EDG-25 (`::handle_contract_violation` attached to a named module) needs
  module support to be tested at all.
* Tests to convert by hand (the importer skips multi-source tests): GCC
  `g++.dg/modules/contract-*` and `std-contract-control_a`,
  `std-module-contract-names`; Clang `Contracts/module-*`,
  `std-module-contracts-names` (P3595/P4283 ones with their papers).
