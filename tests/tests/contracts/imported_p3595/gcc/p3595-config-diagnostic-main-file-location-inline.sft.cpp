//remark: imported from gcc:p3595-config-diagnostic-main-file-location-inline.C
//options_sep:!
//options:--c++26 --contracts '--contract_configuration=[{"output":{"semantic":"enforce","x":1}}]';fn!--c++26 --contracts '--contract_configuration=[{"output":{"semantic":"enforce","x":1}}]';cn!--c++26 --contracts '--contract_configuration=[{"output":{"semantic":"enforce","x":1}}]';cp
//match_regex:unknown key .x. in .output. object
//match_regex:line 30: warning: #warning directive: "located"
// EDG: adapted -- the original's dg-regexp (the configuration's
// diagnostics, then the #warning, with nothing between them) is pinned by
// the recordings instead.  Case 1: the front end alone.  Case 2: a full
// compilation; through edg-gxx the front end stops on the error, and g++,
// run on an empty file, gives the configuration's warning once (EDG-163).
// Case 3: a full compilation without the error; through edg-gxx g++
// compiles the generated C++ and gives the configuration's warning once
// (EDG-162) and its own warning for f at f's line, with no include
// context.  The original's -fcontract-configuration= is the front end's
// --contract_configuration=.
// P3595 (GCC-967): p3595-config-diagnostic-main-file-location.C for the inline
// form, -fcontract-configuration=, whose diagnostics are located at
// <command-line>: here the unknown-key warning.  (A configuration error ends
// the compilation before the main file is read.)  The main file is not
// "included from" <command-line> (no include context on the next diagnostic,
// __INCLUDE_LEVEL__ 0), and the source's diagnostics are at their own lines in
// the main file.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }
// { dg-additional-options {-fcontract-configuration=\[{"output":{"semantic":"enforce","x":1}}\]} }
// { dg-regexp {[^\n]*<command-line>: In JSON value '/0/output/x'\n<command-line>:1:[0-9]+: warning: unknown key 'x' in 'output' object[^\n]*\n[^\n]*p3595-config-diagnostic-main-file-location-inline\.C:15:2: warning: #warning "located"[^\n]*} }

static_assert (__INCLUDE_LEVEL__ == 0);

#warning "located"

#if TEST_NUMBER != 3
int x = ;			// { dg-error "expected primary-expression" }
#else
int f () { }
#endif
