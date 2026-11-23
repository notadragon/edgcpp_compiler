//remark: imported from gcc:p4451-r1-edges-p3400.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400
//match_regex: ", line 51: (?:catastrophic )?error
//match_regex: ", line 52: (?:catastrophic )?error
// p4451-r1-edges.C under -fcontracts-p3400 alone
// (the P3400 label): the P4451 checks step aside and the specifier is
// a label expression.  What P4451 rejects is rejected by P3400's own
// checks, or accepted where P3400 finds a label (<contracts>'s
// contract-control using-directive makes an unqualified `enforced'
// find the standard one).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400" }
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

// Accepted: GCC's nested-name-specifier lookup skips a non-class type and
// finds the namespace alias (stock GCC accepts `using scl = int; scl::x'
// at block scope too), so this is accepted.
struct E1
{
  void f () pre<scl::enforced> (true);
  using scl = int;
};

struct Q { static constexpr auto enforced = std::contracts::controls::enforced; };
void e2 () pre<Q::enforced> (true);

template <auto enforced> void e3 () pre<enforced> (true);

void e4 () pre<> (true);	// { dg-error "expected primary-expression" }
void e4a () { contract_assert<> (true); }	// { dg-error "expected primary-expression" }

struct L { using assertion_control_object = L; };
constexpr L lbl {};
void e5 () pre<lbl> (true);

namespace n6
{
  using namespace std::contracts::controls;
  void e6 () pre<enforced | enforced> (true);
}
