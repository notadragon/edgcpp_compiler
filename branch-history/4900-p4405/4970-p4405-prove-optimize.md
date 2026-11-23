---
id: 4970-p4405-prove-optimize
subject: 'contracts: P4405: --contracts_prove_optimize []'
depends: [4960-p4405-callee-premises]
regenerates: []
fixes: []
---

## Rationale

D4405R0's proof optimization level: our GCC's
`-fcontracts-prove-optimize=<0|1|2|3|follow>` chooses which proving
strategies are on (folding at 0 and up, the proof shadow and assumptions at
1 and up, override shadows and callee premises at 2 and up, explicit flags
winning) and the optimization level the proof shadows run at -- by default
the code-generation level, but never below 2, so an -O0 build proves too
(notadragon_wg21 DECISIONS.md E162-E168, E205, E208).

`--contracts_prove_optimize=` is that option's front end spelling.  The
front end's only proving strategy, folding, is on at every level, so it
records the level and does not consult it; another value is a command-line
error, and the option warns that it has no effect without P4405, whatever
its value, as the other P4405 strategies do.  The drivers map
`-fcontracts-prove-optimize=` to it and pass it on to g++, which makes every
proof through edg_gxx.sh.  eccp-gxx also passes g++'s own strategy options
(`-f[no-]contracts-prove-shadow`, `-f[no-]contracts-prove-assume`) and
`-fcontract-disable-optimized-checks` to g++ unchanged, so GCC tests that
name them can be imported.

The tests are `tests/tests/contracts/p4405optimize/` (the option, its
values and errors; folding at every level with the C-generating back end)
and GCC's proof-level tests in `tests/tests/contracts/imported_p4405optimize/`
(their dump scans and object-code comparisons dropped; GCC's
`contracts-same-diagnostics` variants become extra cases).  GCC's tests
pinned to proof level 0 keep their imports in the earlier entries'
directories, with the option added.

## Compile gap

None: the commit adds its own option.  Intermediate commits of the
generated branch need not build (DECISIONS.md E2, as for GCC and Clang).

## Contents
- src/Changes : /^11\/9\/26 \[\]$/, /^Contracts \(P4405\): --contracts_prove_optimize$/, /^The new --contracts_prove_optimize=<0\|1\|2\|3\|follow> option is accepted for$/, /^our GCC's proof optimization level, to which the drivers pass it on: it$/, /^chooses g\+\+'s proving strategies and how its proof shadows are optimized\.$/, /^The front end's only strategy, folding, is on at every level, so it$/, /^changes nothing there\.  Another value is an error\.  Given without P4405$/, /^it warns that it has no effect, whatever its value\.$/
- src/cmd_line.c : /^  add_option_description\(optk_contracts_prove_optimize,$/, /^      case optk_contracts_prove_optimize:$/, /^  contracts_prove_optimize = -2;$/
- src/cmd_line.h : /^  optk_contracts_prove_optimize,$/, /^\t\tcontracts_prove_optimize;$/
- src/error_msg.txt : /(^|;)ec_cl_invalid_contracts_prove_optimize;/
- util/eccp.sh : /^--contracts_prove_optimize$/, /^         --contracts_prove_optimize \| \\$/, /^          --contracts_prove_optimize=\* \| \\$/
- util/edg_gxx.sh : /^# -fcontracts-prove-optimize= goes to both; the proof optimization level$/, /^    -fcontracts-prove-optimize=\*\)$/
- dev_tools/bin/eccp-gxx : /^#   --contracts_prove_optimize=X -> the g\+\+ spelling \(front end and g\+\+\)$/, /^    --contracts_prove_optimize=\*\)$/, /^#   -f\[no-\]contracts-prove-shadow, -f\[no-\]contracts-prove-assume,$/, /^    -fcontracts-prove-shadow\|-fno-contracts-prove-shadow\|\\$/
- tests/tests/contracts/p4405optimize/* : *
- tests/tests/contracts/imported_p4405optimize/* : *
- tests/expectations/edg_x86_64/contracts.log : /^(p4405optimize|imported_p4405optimize)\//
- tests/expectations/edg_x86_64_gxx/contracts.log : /^(p4405optimize|imported_p4405optimize)\//
