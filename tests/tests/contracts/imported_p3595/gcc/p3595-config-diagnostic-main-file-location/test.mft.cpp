//remark: imported from gcc:p3595-config-diagnostic-main-file-location.C
//source_files: p3595-config-diagnostic-main-file-location.json
//options:--c++26 --contracts --contract_configuration_file=p3595-config-diagnostic-main-file-location.json;fn:--c++26 --contracts --contract_configuration_file=p3595-config-diagnostic-main-file-location.json;cn:--c++26 --contracts --contract_configuration_file=p3595-config-diagnostic-main-file-location.json;cp
//match_regex:unknown key .module. in .match. object
//match_regex:line 32: warning: #warning directive: "located"
// EDG: adapted -- the original's dg-regexp (the configuration's
// diagnostics, then the #warning, with nothing between them) is pinned by
// the recordings instead.  Case 1: the front end alone.  Case 2: a full
// compilation; through edg-gxx the front end stops on the error, and g++,
// run on an empty file, gives the configuration's warning once (EDG-163).
// Case 3: a full compilation without the error; through edg-gxx g++
// compiles the generated C++ and gives the configuration's warning once
// (EDG-162) and its own warning for f at f's line, with no include
// context.  This file is the main file (the original is not included), so
// that __INCLUDE_LEVEL__ is 0 here.
// P3595 (GCC-967): a diagnostic located in the contract configuration --
// here the unknown-key warning -- does not change how the main file is
// entered: the configuration's diagnostics keep their locations in the
// configuration, the main file is not "included from" it (no include
// context on the next diagnostic, __INCLUDE_LEVEL__ 0), and the source's
// diagnostics are at their own lines in the main file.  The dg-regexp
// below takes the configuration's diagnostics and the #warning together,
// so a line between them (such as an include context) fails it.
// The inline form: p3595-config-diagnostic-main-file-location-inline.C.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p3595-config-diagnostic-main-file-location.json" }
// { dg-regexp {[^\n]*p3595-config-diagnostic-main-file-location\.json: In JSON value '/0/match/module'\n[^\n]*p3595-config-diagnostic-main-file-location\.json:2:[0-9]+: warning: unknown key 'module' in 'match' object[^\n]*\n[^\n]*p3595-config-diagnostic-main-file-location\.C:17:2: warning: #warning "located"[^\n]*} }

static_assert (__INCLUDE_LEVEL__ == 0);

#warning "located"

#if TEST_NUMBER != 3
int x = ;			// { dg-error "expected primary-expression" }
#else
int f () { }
#endif
