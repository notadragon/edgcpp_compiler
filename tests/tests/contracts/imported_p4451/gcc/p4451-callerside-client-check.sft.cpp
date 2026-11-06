//remark: imported from gcc:p4451-callerside-client-check.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options_sep: !
//options: --c++26 --contracts_p4451 '--contract_configuration=[{"match":{"caller":true,"kind":"pre"},"output":{"semantic":"enforce"}}]'
//use_system_includes: true
//linker_options: -lcontracts
// EDG: g++ makes the caller-side checks (through edg-gxx); the
// C-generating back end makes none, so this runs under edg_x86_64_gxx
// only.
// EDG: adapted -- the front end has no -fcontracts-client-check=pre;
// P3595's entry for it, a "caller" match of the kind "pre" with the
// semantic enforce, stands in for it.
// D4451R0 "Caller-Side Evaluations" (DECISIONS.md E180) with the
// caller-side checks of -fcontracts-client-check=pre, which checks every
// precondition at the call: the terminating #1 and the ordinary #2 are
// made at the call, then in f, as before E180.  Sequence: 12 12.  Under
// -fcontracts-p4451 alone; the -p3400 and -both files include this one.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p4451 -fcontracts-client-check=pre" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
#include <cstdlib>
#include <cstring>

static char trace[16];

static bool
check (int id)
{
  std::size_t n = std::strlen (trace);
  trace[n] = '0' + id;
  trace[n + 1] = '\0';
  return true;
}

void f ()
  pre<terminating> (check (1))
  pre (check (2)) { }

int
main ()
{
  f ();
  if (std::strcmp (trace, "1212") != 0)
    std::abort ();
}
