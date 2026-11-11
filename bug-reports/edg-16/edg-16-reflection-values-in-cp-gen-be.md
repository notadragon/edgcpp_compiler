# EDG-16: the C++-generating back end renders every reflection value as a null reflection

**Status:** Open upstream; a limitation of the edg-gxx pairing on this branch,
deferred (recorded by the user as a limitation, 2026-10-01).
**Component:** il_to_str.c, `form_reflection` (used by cp_gen_be.c).
**Upstream Link:** None found (searched 2026-10-01); related:
[#45](https://github.com/edgcpp/compiler/issues/45) (cp_gen_be does not render
annotations).
**Affects:** stock EDG at `90b66472e0` (and this branch), `cpfe-cp` with
`--set_flag=reflection`.

## Bug Report

When the C++-generating back end has to print a reflection value -- a
`std::meta::info` constant in an initializer, a template argument, a
`decltype(^^T)`, anything regenerated from the IL rather than printed from a
template's tokens -- it prints the placeholder `(decltype(^^0){})`.  The
source says why:

    /* Reflection values sometimes leak into the C++-generating back end,
       but those values are not actually used.  Render a null reflection
       value. */

They are used, and the placeholder is wrong twice over: it names a null
reflection instead of the entity, and `^^0` (a reflection of a literal) is
not valid under P2996 as adopted, so a compiler that implements reflection
rejects it.  A template's own body is printed from its tokens and keeps
`^^int`, so the output mixes the two.

## Reproducer

[`reflection-value-rendered-as-null.cpp`](reflection-value-rendered-as-null.cpp):

    cpfe-cp --c++26 --set_flag=reflection --gen_c_file_name=out.cpp \
        reflection-value-rendered-as-null.cpp

`out.cpp` contains (line directives removed):

    using info = decltype(((decltype(^^0){})));
    template < info R > struct X { static constexpr bool is_int = ( R == ^^ int ); };
    constexpr info r = (decltype(^^0){});
    static_assert(X< (decltype(^^0){})> ::is_int);

which our GCC (with `-freflection`) rejects: "'^^' cannot be applied to this
operand"; with the placeholder accepted, the `static_assert` would fail.

## Notes

On this branch it means no program that uses reflection compiles through
`edg-gxx`: with reflection on, EDG defines `__cpp_impl_reflection`, so
libstdc++'s `<type_traits>` enables its `is_reflection` specializations, and
their `decltype(^^int)` comes out as the placeholder.  (`edg-gxx` also does
not map `-freflection` to `--set_flag=reflection`, and EDG's reflection
library, `<experimental/meta>`, is a test header pack --
`tests/tests/reflections/.includes/exp_meta` -- not on its include path.)
A fix needs a C++ spelling for every kind of reflection value (`^^T`,
`^^name` with the qualification the context needs, the reflection of a
value, of a base, of a data member description, ...), much as the back end
already spells other constants.
