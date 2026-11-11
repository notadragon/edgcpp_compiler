---
id: 6990-contract-violation-information
subject: 'contracts: --contract_violation_information=full|none []'
depends: [6900-cross-paper-options, 4940-p4405-override]
regenerates: []
fixes: []
---

## Rationale

A contract-violation record names the file, function and line of the
assertion and carries its comment, so that text lands in the object file.
`--contract_violation_information=none` (as GCC's and Clang's
`-fcontract-violation-information=none`, notadragon_wg21 DECISIONS.md E170)
keeps it out; `full`, the default, records everything as before.  The
option is accepted in C and C++, whatever the other contract options, and
the drivers pass it on to g++ as `-fcontract-violation-information=`.

Through edg_gxx.sh g++ builds every record (cpfe-cp puts the contract
assertions out as written), so the front end's part there is the
preprocessor's: under `none`, in GNU and Clang modes, it predefines the
emulated compiler's `__gcc_contracts_violation_information_none` (1)
or `__clang_contracts_violation_information_none` (1), on which libstdc++'s
`<cassert>` contract path, its P3290 manual API and `<contracts.h>` pass an
empty location and comment.

With the C-generating back end the front end builds the records of the
noexcept semantics' checks itself (`contract_noexcept_violation_call`, the
only checks that report a violation there): under `none` their comment,
file name and function name are empty and their line is 0 (their column is
always 0), they carry no P3099 message, and a P3400 label computes no
comment or message for them (its local handler and query function are
passed all the same, 4810-p4298-c-gen).  No implicit assertion reaches those
records.  Constant evaluation's diagnostics are unchanged.
The default violation handler is libcontracts' and libstdc++exp's, from
the paired GCC, which leaves out empty location parts.

The tests are `tests/tests/contracts/vinfo/`: GCC's and Clang's
violation-information tests mirrored for edg-gxx (records, the default
handler, the object's contents, P3100, the P3290 manual API and
`<contracts.h>`, the latter in C++ since the suite cannot test C), and for
both back ends with the noexcept semantics (records, the default handler,
the object's contents with the C-generating back end), the option and its
macros, and a constant-evaluation diagnostic under `none`.

## Compile gap

None.

## Contents

- src/Changes : /^11\/2\/26 \[\]$/
- src/Changes : /^Contracts: --contract_violation_information=full\|none$/
- src/Changes : /^The new --contract_violation_information option/
- src/Changes : /^__gcc_contracts_violation_information_none \(Clang:$/
- src/Changes : /^C-generating back end every contract-violation record has an empty file$/
- src/cmd_line.c : #2:17, #7:25, #13:16
- src/cmd_line.h : #1:17, /^\t\tcontract_violation_information_none;$/
- src/error_msg.txt : /(^|;)ec_cl_invalid_contract_violation_information;/
- src/macro.c : #6:1
- src/statements.c : @expression:1, @NULL:1
- util/eccp.sh : #1:4, #7:3, #9:3
- util/edg_gxx.sh : #0:18
- dev_tools/bin/eccp-gxx : #0:18
- tests/tests/contracts/vinfo/* : *
- tests/expectations/edg_x86_64/contracts.log : /^vinfo\//
- tests/expectations/edg_x86_64_gxx/contracts.log : /^vinfo\//
