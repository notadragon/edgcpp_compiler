---
id: 0160-float-literal-exponent-range
subject: 'Treat a floating literal with a huge exponent as overflow or underflow []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A fix to upstream EDG, independent of contracts.  With the software
floating-point conversion (`FP_USE_EMULATION`, as in the Release
configuration), `split_string` limits the precision of a decimal literal so
that the conversion's bignums cannot overflow, by `BIGINT_MAX_EXP` minus the
exponent; an exponent beyond `BIGINT_MAX_EXP` made that negative, and the
conversion then failed an assertion in `bigint_mult` (whose diagnostic in
turn failed in `write_orig_source_line`: "bad lexical escape").  A decimal
exponent of at least `BIGINT_MAX_EXP` now means overflow and one of at most
`-BIGINT_MAX_EXP` underflow: `BIGINT_MAX_BITS` is the largest format's
maximum exponent plus twice its mantissa, so such a value is beyond every
format's range or below every denormal.  The literal is then out of range like
`1.0e400`; a raw literal operator, which receives the characters, accepts
it, as the imported GCC test `cpp0x/udlit-overflow` expects.

Tracked in `bug-reports/` (EDG-64) until upstream fixes it.

## Compile gap

None.

## Contents

- src/floating.c : *
- tests/tests/contracts/sema/*float_literal_exponent_range* : *
- tests/expectations/edg_x86_64_gxx/contracts.log : /^sema\/float_literal_exponent_range/
