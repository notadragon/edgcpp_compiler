//remark: imported from gcc:p4451-r1-edges.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p4451
//match_regex: ", line 57: (?:catastrophic )?error
//match_regex: ", line 59: (?:catastrophic )?error
//match_regex: ", line 61: (?:catastrophic )?error
//match_regex: ", line 62: (?:catastrophic )?error
//match_regex: ", line 66: (?:catastrophic )?error
//match_regex: ", line 71: (?:catastrophic )?error
// EDG: adapted -- E1's xfail dg-error is dropped: in GNU mode the front end,
// like g++, skips a non-class type in the lookup of the name before `::'
// and accepts E1, as the comment above it says, recorded so.
// P4451 [basic.contract.control] (D4451R1): more cases of the lookup rule.
// Accepted: lookup of the name before `::' ignores a block-scope variable
// named `std', and later class members named `std', `enforced' or `scl'
// do not change what the qualified or alias spelling finds.  Rejected,
// once each: a later member type alias `scl' hides the namespace alias in
// complete-class context; a class member holding a copy of the label; a
// template parameter named `enforced'; an empty specifier; a P3400-style
// label object; `enforced | enforced'.  Under -fcontracts-p4451
// alone.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p4451" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

namespace scl = std::contracts::controls;

void ok1 (int x)
{
  int std = x;
  contract_assert<std::contracts::controls::enforced> (std > 0);
}

struct OK2
{
  void f () pre<std::contracts::controls::enforced> (true);
  void g () pre<scl::enforced> (true);
  void h () { contract_assert<scl::enforced> (true); }
  int std;
  int enforced;
  int scl;
};

// XFAIL: GCC's nested-name-specifier lookup skips a non-class type and
// finds the namespace alias (stock GCC accepts `using scl = int; scl::x'
// at block scope too), so this is accepted.
struct E1
{
  void f () pre<scl::enforced> (true);
  using scl = int;
};

struct Q { static constexpr auto enforced = std::contracts::controls::enforced; };
void e2 () pre<Q::enforced> (true);	// { dg-error "must name .std::contracts::controls::enforced." }

template <auto enforced> void e3 () pre<enforced> (true);	// { dg-error "must name .std::contracts::controls::enforced." }

void e4 () pre<> (true);	// { dg-error "must name .std::contracts::controls::enforced." }
void e4a () { contract_assert<> (true); }	// { dg-error "must name .std::contracts::controls::enforced." }

struct L { using assertion_control_object = L; };
constexpr L lbl {};
void e5 () pre<lbl> (true);	// { dg-error "must name .std::contracts::controls::enforced." }

namespace n6
{
  using namespace std::contracts::controls;
  void e6 () pre<enforced | enforced> (true);	// { dg-error "must name .std::contracts::controls::enforced." }
}
