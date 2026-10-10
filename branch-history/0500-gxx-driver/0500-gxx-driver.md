---
id: 0500-gxx-driver
subject: 'edg_gxx.sh: pair the C++-generating front end with g++ []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A contracts implementation is only useful if its programs run against a real
standard library and a real contract-violation runtime, and EDG supplies
neither.  The pairing that provides both is a GNU g++: its libstdc++, and the
libcontracts runtime that implements the shared contract-violation ABI.

eccp, EDG's own driver, cannot make that pairing work.  It lowers to C, and
the C-generating back end implements exception handling with setjmp and
longjmp, so code it compiles cannot catch an exception thrown by g++-compiled
code (libstdc++'s `std::__throw_out_of_range`, or a throwing violation
handler), nor let one of its own pass through a g++-compiled frame.

`util/edg_gxx.sh` runs the C++-generating front end instead and hands the
regenerated C++ to g++, so exception handling, RTTI and the rest of the
Itanium ABI are g++'s own.  It accepts g++'s options and routes the ones that
affect how the source is interpreted (`-D`, `-U`, `-I`, `-isystem`,
`-include`, `-std=`, `-f[no-]exceptions`, `-f[no-]rtti`) to the front end,
since g++ sees only already-preprocessed, already-checked output.  The paired
g++'s version and include search list are read from it on each run, so one
install works against any g++.  When the front end fails, its exit status is
the driver's, so a catastrophic error stays distinguishable from an ordinary
one.

A reviewer should check the option routing: an option sent only to g++ that
changes the meaning of the source would make the front end check a different
program from the one g++ compiles.

## Compile gap

None.  A new script with no dependency on anything else in the series.

## Contents

- util/edg_gxx.sh : #0, #0:0, #0:14
- util/Changes : @s:1, /^edg_gxx\.sh: pair the C\+\+-generating front end with g\+\+$/, /^A new driver, util\/edg_gxx\.sh, compiles C\+\+ by running/, /^front end \(cpfe-cp\) and handing the regenerated C\+\+ to a GNU g\+\+\.  It takes$/, /^g\+\+'s options\.  Because g\+\+ compiles the result, exception handling, RTTI$/, /^and the rest of the Itanium ABI are g\+\+'s own: code compiled this way throws$/, /^through, catches from and links against g\+\+-compiled libraries \(including$/, /^The g\+\+ used is \$EDG_GXX, else/
