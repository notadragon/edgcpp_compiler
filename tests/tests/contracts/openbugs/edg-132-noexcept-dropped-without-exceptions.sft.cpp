//remark:contracts: EDG-132 (stock): with exceptions disabled cpfe-cp puts out no exception specification on a function, so g++ rejects a valid program that relies on one
//require:BACK_END_IS_CP_GEN_BE 1
//type:cn
//options_all:--no_exceptions
//match_regex:Test_name.c:12:.*error: invalid conversion from .void \(\*\)\(\). to .void \(\*\)\(\) noexcept.
//match_regex:Test_name.c:13:.*error: static assertion failed
// Records the current (wrong) behavior: g++ -fno-exceptions accepts both
// lines, but the C++ cpfe-cp generates declares g without its noexcept
// (the pointer's type keeps it).  The front end accepts the program; the
// errors are g++'s.  A fix shows up as a deviation.
void g() noexcept {}
void (*p)() noexcept = &g;
static_assert(noexcept(g()));
