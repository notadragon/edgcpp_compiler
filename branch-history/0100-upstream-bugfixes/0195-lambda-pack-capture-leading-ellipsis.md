---
id: 0195-lambda-pack-capture-leading-ellipsis
subject: 'Reject a lambda simple-capture with the ellipsis before the name []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A fix to upstream EDG, independent of contracts.  A simple-capture is
`identifier ...opt` ([expr.prim.lambda.capture]); only an init-capture puts
the ellipsis first.  `scan_lambda_capture_list` took a leading ellipsis as
that of a pack init-capture, so `[...xs]` (and `[&...xs]`) was accepted as
`[xs...]`, and `[...xs...]` was "expected a ']'".  An ellipsis followed by
an identifier that has no initializer is now diagnosed ('a pack capture with
no initializer is written "xs...", not "...xs"'), once per capture, and the
capture is recovered as `xs...`, a second ellipsis after the name being
skipped.  `[...xs... = xs]`, an init-capture with an ellipsis too many, is
still "expected a ']'".  GCC and Clang reject the same captures.  The
recordings of Clang's imported `expr.prim.lambda/p23` gain the two errors
its `expected-error` lines ask for, and GCC's imported `cpp2a/lambda-pack-init2`
reports its two `[...xs...]` captures with the new message.

Tracked in `bug-reports/` (EDG-118) until upstream fixes it.

## Compile gap

None.

## Contents

- src/class_decl.c : /leading_ellipsis_simple_capture/
- src/error_msg.txt : /(^|;)ec_lambda_pack_capture_ellipsis_before_name;/
- tests/tests/contracts/sema/*lambda_pack_capture_leading_ellipsis* : *
- tests/tests/imported/clang/cpp/CXX/expr/expr.prim/expr.prim.lambda/.p23.rto/* : *
- tests/tests/imported/gnu/cpp/cpp2a/.lambda-pack-init2.rto/* : *
