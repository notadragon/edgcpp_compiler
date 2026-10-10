# EDG-64: a floating literal with a huge exponent is an internal error under the software floating-point conversion

**Status:** Open upstream; fixed on this branch (band 0100, `split_string`
in floating.c).  Not contracts-specific.
**Component:** floating.c `split_string`; the assertion fails in
`bigint_mult` (via `fp_string_to_float`, `dec2bin`, `bigint_mult_pow5`, from
the lexer's `conv_float_literal`), and reporting it fails in error.c
`write_orig_source_line` ("bad lexical escape", line 2694 at `0ac366374c`).
**Upstream Link:** None found (searched 2026-10-02).
**Affects:** stock EDG at `0ac366374c` with the software floating-point
conversion (`FP_USE_EMULATION`, the Release configuration); EDG's
recordings come from the Debug configuration.
**Found:** M6 baseline triage, 2026-10-02: imported GCC test
`cpp0x/udlit-overflow`.
**Test:** `tests/tests/contracts/sema/float_literal_exponent_range.sft.cpp`.

## Bug Report

Generating C for a raw literal operator applied to a floating literal whose
exponent overflows stops with "Internal error loop: assertion failed:
write_orig_source_line: bad lexical escape".  The program is valid: a raw
literal operator receives the literal's characters, so the exponent's range
does not matter ([lex.ext]).  The integer form
(`12345678901234567890123456789012345678901234567890_w`) is fine.

## Reproducer

[`udl-float-exponent.cpp`](udl-float-exponent.cpp):

    cpfe --c++11 --gnu_version=999999 --gen_c_file_name=out.c udl-float-exponent.cpp

## Fix (this branch)

`split_string` limits a decimal literal's precision so that the bignums of
the conversion stay in range, by `BIGINT_MAX_EXP` minus the exponent, which
an exponent beyond `BIGINT_MAX_EXP` made negative.  A decimal exponent of
at least `BIGINT_MAX_EXP` now means overflow, one of at most
`-BIGINT_MAX_EXP` underflow (`BIGINT_MAX_BITS` is the largest format's
maximum exponent plus twice its mantissa, so such a value is beyond every
format's range or below every denormal): the literal is out of range like
`1.0e400`, and a raw literal operator accepts it.
