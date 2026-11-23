//remark: imported from gcc:p4405-premise-semantic-copies.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4405-premise-semantic.json
//options: --c++26 --contracts --contracts_p4405 --contracts_p4298 --contracts_allow_assume --contract_configuration_file=p4405-premise-semantic.json -O2
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// P4405 premises decided by this TU: p4405-premise-semantic.C's ignore,
// observe and noexcept_observe callees with premise copies
// (-fcontracts-prove-callee-premises, DECISIONS.md E199 (b)).  An inline
// callee's observe or noexcept_observe precondition is a premise for the
// consumer through the callee's copy when the premise shadow inlines it
// (always_inline), proven conditionally; a noinline copy lends nothing, and
// an ignore precondition leaves nothing in the callee (GCC-885).  The
// ordinary shadow's results are those of p4405-premise-semantic.C.
//
// Mirror: clang/test/Contracts/p4405-premise-semantic.cpp (its copy runs)
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontracts-p4298 -fcontracts-allow-assume -O2" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4405-premise-semantic.json" }

int sink (int);

namespace sem_ignore_always {
[[gnu::always_inline]] inline void h (int x) pre (x > 0) { sink (x); }
}

int d_ignore_always (int x)
{
  sem_ignore_always::h (x);
  contract_assert (x > 0);	// { dg-warning "contract assertion cannot be proven" }
  return 0;
}

namespace sem_ignore_never {
[[gnu::noinline]] inline void h (int x) pre (x > 0) { sink (x); }
}

int d_ignore_never (int x)
{
  sem_ignore_never::h (x);
  contract_assert (x > 0);	// { dg-warning "contract assertion cannot be proven" }
  return 0;
}

namespace sem_observe_always {
[[gnu::always_inline]] inline void h (int x) pre (x > 0) { sink (x); }
}

int d_observe_always (int x)
{
  sem_observe_always::h (x);
  contract_assert (x > 0);	// { dg-bogus "cannot be proven" }
  return 0;
}

namespace sem_observe_never {
[[gnu::noinline]] inline void h (int x) pre (x > 0) { sink (x); }
}

int d_observe_never (int x)
{
  sem_observe_never::h (x);
  contract_assert (x > 0);	// { dg-warning "contract assertion cannot be proven" }
  return 0;
}

namespace sem_noexcept_observe_always {
[[gnu::always_inline]] inline void h (int x) pre (x > 0) { sink (x); }
}

int d_noexcept_observe_always (int x)
{
  sem_noexcept_observe_always::h (x);
  contract_assert (x > 0);	// { dg-bogus "cannot be proven" }
  return 0;
}

namespace sem_noexcept_observe_never {
[[gnu::noinline]] inline void h (int x) pre (x > 0) { sink (x); }
}

int d_noexcept_observe_never (int x)
{
  sem_noexcept_observe_never::h (x);
  contract_assert (x > 0);	// { dg-warning "contract assertion cannot be proven" }
  return 0;
}
