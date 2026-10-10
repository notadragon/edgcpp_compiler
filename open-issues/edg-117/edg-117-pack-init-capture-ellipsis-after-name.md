# EDG-117: a P3098 init-capture with its ellipsis after the name is misdiagnosed and dropped

**Status:** Open; this branch only (contracts are not in upstream EDG).
**Component:** declarator.c (`scan_postcondition_captures`,
`scan_postcondition_capture`), band 4000.
**Found:** M21-EDG, 2026-10-09, mirroring GCC's and Clang's
`p3098-unexpanded-pack-capture` (EDG-115, EDG-116; GCC-681, CLANG-663).
**Watch test:** `tests/tests/contracts/openbugs/edg-117-pack-init-capture-ellipsis-after-name.sft.cpp`.

## Reproducer

    template <class... T>
    int bad3(T... xs) post [y... = xs] (((y > 0) && ...)) { return 0; }
    template <class... T>
    int bad4(T... xs) post [...y... = xs] (((y > 0) && ...)) { return 0; }
    template <class... T>
    int d7(T... xs) post [y... = xs, z = 0] (((y > z) && ...));

    edg-gxx -std=c++26 -fcontracts-p3098 -fsyntax-only t.cpp

The ellipsis of a pack init-capture goes before the name (D3098R3's
grammar, as in a lambda): each capture is ill-formed and is rejected, but
`[y... = xs]` is reported as "only a function parameter can be captured
without an initializer in a postcondition" (it has one), and
`[...y... = xs]` as "expected an identifier in a postcondition capture" plus
"pack expansion does not make use of any argument packs".  The capture is
then dropped, so every use of `y` in the predicate adds "identifier "y" is
undefined" and "fold expression does not refer to a parameter pack" (three
or four errors a capture).  The rest of the list is still parsed (`z` is
declared).

Expected, as GCC (GCC-681) and Clang (CLANG-663) do: one error saying the
ellipsis goes before the name (`...y =`), and the capture recovered as the
pack `...y = xs`, so the predicate adds nothing.

Stock EDG's lambda gives the same kind of cascade for `[y... = xs]`
("expected a "]"", `y` undefined); the misleading message is the
postcondition parser's own.  The simple-capture forms (`[...xs]`,
`[...xs...]`) get "expected an identifier" and the second "pack expansion"
error; that recovery is pinned by `imported_p3098/gcc/p3098-unexpanded-pack-capture`
and is not this issue.
