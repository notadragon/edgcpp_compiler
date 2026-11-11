# The C-generating back end drops `extern inline` from a `gnu_inline` function

**Status:** Open upstream and on this branch; deferred by the user
2026-10-05.  Reproduces on stock EDG (`upstream/main`), not
contracts-related.  Found 2026-10-03 by the full edgy run after EDG-67's
fix let imported GCC `c/va-arg-pack-len-1` compile far enough to reach it.
**Component:** C-generating back end (c_gen_be.c), GNU mode.

## Reproducer

`gnu-inline-va-arg-pack.c`:

    extern int g(int, ...);
    extern inline __attribute__((always_inline, gnu_inline)) int
    f(int a, ...)
    {
      return g(a, __builtin_va_arg_pack());
    }
    int g(int a, ...) { return a; }
    int main(void) { return f(0, 1, 2); }

    eccp --gnu_version=170000 gnu-inline-va-arg-pack.c

    eccp: diagnostics generated from compilation of gnu-inline-va-arg-pack.int.c:
    gnu-inline-va-arg-pack.c:5:51: error: invalid use of '__builtin_va_arg_pack ()'

gcc compiles and runs the source as is (`gcc -O2`).  The generated C puts
the definition out as

    __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) int f( int __3_7_a, ...)

without `extern inline`, so gcc compiles an out-of-line copy of `f`, in
which `__builtin_va_arg_pack` is invalid (it is allowed only in a function
that is always inlined and never emitted).  The same happens in C++ mode.

## Notes

With `extern inline` and `gnu_inline`, GNU C emits no out-of-line
definition (the GNU89 inline semantics); the generated C must keep both, or
drop the definition.  Watch test:
`tests/tests/contracts/openbugs/edg-78-gnu-inline.sft.cpp` (its recording
holds the failed gcc compilation).
