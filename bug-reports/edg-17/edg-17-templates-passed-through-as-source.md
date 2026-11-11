# EDG-17: the C++-generating back end passes templates to the downstream compiler as source

**Status:** Open upstream (by design); a limitation of the edg-gxx pairing on
this branch, deferred (DECISIONS.md E6 in notadragon_wg21, answered
2026-10-01: accept it for now, revisit with the defect's resolution).
**Component:** cp_gen_be.c, `gen_template` and
`template_should_be_generated_from_prototype_instantiation`.
**Upstream Link:** None found (searched 2026-10-01); related:
[#69](https://github.com/edgcpp/compiler/issues/69) ("ALL_TEMPLATE_INFO_IN_IL
should default to TRUE in cp_gen_be modes").
**Affects:** stock EDG at `90b66472e0` (and this branch), `cpfe-cp`.

## Bug Report

`cpfe-cp` prints every template -- function templates, class templates and
their members, member templates, generic lambdas -- from its recorded
tokens, and its instantiations are left to the compiler that compiles the
output; EDG's own instances are not printed.  (`ALL_TEMPLATE_INFO_IN_IL` is
FALSE in this configuration, so templates are not generated from their
prototype instantiations, and generating explicit specializations for
implicit instances -- `NONCLASS_TEMPLATE_INSTANTIATIONS_IN_SOURCE_SEQUENCE_LISTS`
-- is deprecated: host_envir.h stops with an `#error`.)  Non-template code
is regenerated from the IL.

So everything inside a template must be accepted, with the same meaning, by
the downstream compiler: its language level, its extensions, and every
experimental feature EDG implements.  EDG's semantic analysis of the
template still runs, but the code that runs is the downstream compiler's
instantiation of the text.

## Reproducer

[`template-passed-through-as-source.cpp`](template-passed-through-as-source.cpp):

    cpfe-cp --c++26 --gen_c_file_name=out.cpp template-passed-through-as-source.cpp

`out.cpp` contains (line directives removed):

    template < class ... T > auto first ( T ... x ) { return x ... [ 0 ] + 1 + 2; }
    int f(int a) { return (a + 1) + 2; }

the template as its tokens (pack indexing and all), the function as
regenerated IL.

## Notes

On this branch contract assertions reach g++ as written everywhere, by
design (the C++-generating back end leaves them to the compiler of its
output), so for them this changes nothing.  Reflection inside templates
depends on g++'s support (and see EDG-16 for reflection values outside
them).
Upstream #69 points at the way out: printing templates from their prototype
instantiations.
