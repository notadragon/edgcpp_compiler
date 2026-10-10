# Open Upstream Bugs

Bugs found during this implementation that reproduce on stock upstream EDG
(`upstream/main`, github.com/edgcpp/compiler), independent of anything in
this branch.  Each links to a self-contained writeup plus a reproducer.  A
row is removed (and its files deleted) once the bug is fixed upstream,
regardless of who fixed it or whether it was ever formally filed.  Nothing
here is filed by an agent: filing upstream is the user's.

**Upstream Link** distinguishes three states: a link means an issue exists;
`None found (searched <date>)` means upstream's issue tracker was searched
and nothing matched; `UNKNOWN` means nobody has looked yet.

For what is broken on **this branch** right now -- including branch-only
issues that never reproduce upstream, which for contracts is all of them --
see [`../open-issues/README.md`](../open-issues/README.md).  That file also
carries the `Next ID` line both directories allocate from.

## Layout: the `edg-N-` prefix means "do not attach this"

One directory per bug, `edg-N/`, as in the GCC and Clang forks, and within
it the filename prefix says whether a file is for upstream or for us: an
unprefixed name is meant to go on the issue, a prefixed one is ours.  Our
numbering is internal, so it stays out of any file someone upstream reads.

| Bug | Summary | Status | Upstream Link | Details |
|-----|---------|--------|----------------|---------|
| EDG-16 | The C++-generating back end renders every reflection value as a null reflection, `(decltype(^^0){})`, which is wrong and not valid C++ | Open (limitation) | None found (searched 2026-10-01); related [#45](https://github.com/edgcpp/compiler/issues/45) | [edg-16](edg-16/edg-16-reflection-values-in-cp-gen-be.md) |
| EDG-17 | The C++-generating back end prints templates from their tokens and leaves instantiation to the downstream compiler, so everything in a template (contracts, reflection, ...) depends on that compiler's support | Open (limitation) | None found (searched 2026-10-01); related [#69](https://github.com/edgcpp/compiler/issues/69) | [edg-17](edg-17/edg-17-templates-passed-through-as-source.md) |
| EDG-44 | Constant evaluation accepts destroying an object twice and writing to an object after its destruction | Fixed here | None found (searched 2026-10-01) | [edg-44](edg-44/edg-44-constexpr-double-destroy.md) |
| EDG-45 | Constant evaluation accepts referring to a member of an object before its constructor begins ([class.cdtor]/1) | Fixed here | None found (searched 2026-10-01) | [edg-45](edg-45/edg-45-member-address-before-ctor.md) |
| EDG-51 | An out-of-class definition of a member function of a class template, or of a member function template, takes its parameters' top-level const from the in-class declaration | Fixed here | None found (searched 2026-10-01) | [edg-51](edg-51/edg-51-const-param-out-of-class-member.md) |
| EDG-52 | With the C-generating back end, the operand of `[[assume(...)]]` is evaluated at run time when the call at its top is inlined (the assumption's statement is replaced with the inlined code), and the attribute is lost | Fixed here | None found (searched 2026-10-01) | [edg-52](edg-52/edg-52-assume-operand-inlined.md) |
| EDG-56 | An IL file cannot be read back (cdisp asserts in `disp_ptr_value`) when the translation unit has an attribute, an inline variable, a local with a member whose destructor is not trivial, or certain requires-clauses | Open | None found (searched 2026-10-02) | [edg-56](edg-56/edg-56-il-file-round-trip.md) |
| EDG-58 | `typeid` of a polymorphic glvalue whose dynamic type is statically known is treated as an unevaluated operand: the variable is not odr-used (a lambda names it without capturing it) | Fixed here | None found (searched 2026-10-02) | [edg-58](edg-58/edg-58-typeid-known-type-unevaluated.md) |
| EDG-61 | 32 IL-display tests pipe their output into `enumerate_addrs` or `normalize_test_output`, which are not commands, so their recordings hold bash's error message | Fixed here | None found (searched 2026-10-02) | [edg-61](edg-61/edg-61-test-filter-tool-names.md) |
| EDG-62 | Lowering a multidimensional VLA with an initializer, or a C VLA parameter sized by `sizeof` of another, is an internal error (`find_vla_dimension: not found`); Release configuration only reaches it | Open (deferred here) | None found (searched 2026-10-02) | [edg-62](edg-62/edg-62-find-vla-dimension.md) |
| EDG-63 | Lowering `__typeof` of a dereferenced pointer-to-VLA cast in a comma expression is an internal error (`vla_size_expr`) | Open (deferred here) | None found (searched 2026-10-02) | [edg-63](edg-63/edg-63-vla-size-expr.md) |
| EDG-64 | A floating literal with a huge exponent is an internal error under the software floating-point conversion (reported as `write_orig_source_line: bad lexical escape`) | Fixed here | None found (searched 2026-10-02) | [edg-64](edg-64/edg-64-udl-float-exponent.md) |
| EDG-66 | An immediate invocation in the unselected operand of a conditional with a constant condition does not fold its source location default arguments ("did not produce a valid constant expression" for `std::source_location::current()`) | Fixed here | None found (searched 2026-10-03) | [edg-66](edg-66/edg-66-source-location-unselected-operand.md) |
| EDG-78 | The C-generating back end puts out an `extern inline` `gnu_inline` function without `extern inline`, so gcc compiles an out-of-line copy (where `__builtin_va_arg_pack` is invalid) | Open | None found (searched 2026-10-03) | [edg-78](edg-78/edg-78-gnu-inline-extern-inline-dropped.md) |
| EDG-67 | `__builtin_c23_va_start`, which the `va_start` of GCC 15+ `<stdarg.h>` (C23) and GCC 16+ `<cstdarg>` (C++26) expands to, is unknown | Fixed here | None found (searched 2026-10-03) | [edg-67](edg-67/edg-67-c23-va-start.md) |
| EDG-70 | With exceptions disabled, the noexcept operator (and the nothrow traits) is true for every expression; g++ and Clang still answer from the exception specifications | Open | None found (searched 2026-10-03) | [edg-70](edg-70/edg-70-noexcept-without-exceptions.md) |
| EDG-81 | In a default member initializer, a lambda capturing `this` from an enclosing lambda that captured `*this` by copy is an internal error (`type_pointed_to: not a pointer type`) | Fixed here | None found (searched 2026-10-04) | [edg-81](edg-81/edg-81-nested-this-capture-of-copy.md) |
| EDG-82 | With the C-generating back end, a lambda capturing `*this` nested in another in a default member initializer (with another member initializer's lambda in the class) is an internal error when local type names are mangled (`enclosing_routine_for_local_type`) | Open | None found (searched 2026-10-04) | [edg-82](edg-82/edg-82-nested-star-this-mangling.md) |
| EDG-111 | A requires-clause on a member typedef of function type in a class template is accepted, though a requires-clause is allowed only on a declarator of a templated function | Open | None found (searched 2026-10-09) | [edg-111](edg-111/edg-111-requires-clause-member-typedef.md) |
| EDG-118 | A lambda simple-capture with the ellipsis before the name (`[...xs]`) is accepted, though only an init-capture puts the ellipsis first | Open | None found (searched 2026-10-09) | [edg-118](edg-118/edg-118-lambda-pack-capture-leading-ellipsis.md) |
