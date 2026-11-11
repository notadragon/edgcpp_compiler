# EDG-143: a reference of static storage duration bound to an array of class temporaries breaks the IL round trip (Debug)

**Status:** Open upstream and on this branch.  Not contracts-specific.
**Component:** IL memory regions (the dynamic initialization of the
lifetime-extended array temporary), seen by the checking configuration's IL
write/read round trip.
**Upstream Link:** None found (searched 2026-10-27).
**Affects:** stock EDG at `35b7c4776c` (upstream/main), Debug configuration
(the IL round trip; the Release configuration does not check it), C++17 and
later.
**Found:** AE-EDG, 2026-10-27, by the Debug contracts run of three imports of
gnu_gcc's D4405 tests (`imported_p4405/gcc/p4405-analysis-static-array-dtor`,
`-static-array-dtor-O2`, `p4405-analysis-address-static-temp`), whose
Release runs pass.

## Bug Report

    struct DI { ~DI (); };
    const DI (&ra)[2] = {DI (), DI ()};

compiled by the Debug front end stops with

    IL entry write-read difference: region number   1
         entry kind = 32 (dynamic-init), written = N, read = N-1
            missing entry = ...
    Internal error: read_memory_region: not all expected entries were read

A dynamic initialization of the extended temporary is allocated where the
written IL does not reach it.  The same with `std::initializer_list<DI>` in
place of the array reference is accepted.

## Reproducer

With a Debug build (`BUILD_TYPE=Debug`), `setarch $(uname -m) -R` and either
front end, e.g. through our driver:

    EDG_BUILD=<debug build> setarch $(uname -m) -R bin/edg/run.sh t.cpp -- -std=c++17 -c

(notadragon_wg21's `bin/edg/run.sh`; at stock `35b7c4776c` the C++-generating
front end shows the same error.)
