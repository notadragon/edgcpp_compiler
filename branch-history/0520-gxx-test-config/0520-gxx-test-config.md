---
id: 0520-gxx-test-config
subject: 'edgy: add a test configuration that runs through edg_gxx.sh []'
depends: [0500-gxx-driver]
regenerates: []
fixes: []
---

## Rationale

The contracts tests have to run their programs against g++'s libstdc++ and
the libcontracts violation runtime, and see exceptions cross between EDG- and
g++-compiled code.  Neither of edgy's configurations can do that: both end
in the C-generating back end, whose setjmp/longjmp exception handling cannot
catch an exception thrown by g++-compiled code.

This adds a third kind of configuration, `gxx`, and one instance of it,
`edg_x86_64_gxx`.  edgy always runs `eccp`, and `dev_tools/bin/eccp` defers
to `$ECCP`, so the configuration sets `ECCP` to a new adapter,
`dev_tools/bin/eccp-gxx`, which turns the harness's eccp command lines
(`--cpfe_only`, `-c`, or a link to `a.out`, with EDG options) into
`edg_gxx.sh` command lines; the front end binary under test is `CPFE_CP`.

Two restrictions keep the new configuration from multiplying the run: a
variant's `suites` lists the only suites it runs, and the top-level
`suite_configs` lists, per suite, the only configurations that run it.
`edg_x86_64_gxx` runs only the new `contracts` suite, which runs under it and
under `edg_x86_64` (its front end tests, IL display among them, need the
C-generating front end); the configuration joins the default set, so
`edgy all` runs every other suite exactly as before.  A test that needs one
of the two configurations says so with `//require:` on the build
configuration (`BACK_END_IS_CP_GEN_BE 1` or `DO_IL_LOWERING 1`).

The suite starts with three tests of the configuration itself: a catch of a
libstdc++ exception (gxx only; it fails under eccp), a runtime-negative exit
status, and front end only, compile and link modes.

Under the new configuration the library headers are the paired GCC
install's libstdc++, whose location, GCC version and line numbers vary from
one install to the next.  edgy's output normalizer therefore rewrites, for
gxx configurations only, a quoted path into that install's
`include/c++/<version>/` to `GXX_INCLUDE_CXX/`, and the line number the front
end pairs with such a path (`declared at line N of "PATH"`, `"PATH", line N`)
to `line N`, and likewise such a path unquoted, as g++ reports positions in
those headers (`PATH:LINE:COLUMN: ...`), with its line and column, so that a
recording does not change when those headers do (the source lines g++ echoes
from them are left as they are).

The `contracts` suite makes `edg_x86_64_gxx` its default configuration, so
that the recordings made through g++ are its `default` ones and
`edg_x86_64` inherits its expectations and records only where its output
differs.  It says so in `tests/tests/contracts/.run_test_env`.  edgy does not
source that file (that would run a shell script from the test tree), so it
reads it as data: blank lines, comments and plain `[export ]NAME=VALUE`
assignments, overriding the inherited environment; anything else is an
error.

A reviewer should check the adapter's option routing, in particular that the
harness's `Test_name.c` copies are compiled as C++ by the front end rather
than passed straight to g++.

## Compile gap

None.  Test tooling only; no front end change.

## Contents

- dev_tools/bin/edgy : *
- dev_tools/bin/eccp-gxx : #0, #0:0, #0:16
- dev_tools/pylibs/edgtest/__init__.py : *
- dev_tools/pylibs/edgtest/filter/__init__.py : *
- tests/.edgy/config.json : *
- tests/expectations/edg_x86_64_gxx/README.md : *
- tests/expectations/edg_x86_64/contracts.log : /^harness\//
- tests/tests/contracts/.run_test_env : *
- tests/tests/contracts/.run_test_flags : *
- tests/tests/contracts/harness/* : *
