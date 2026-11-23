//remark: imported from gcc:p4451-lookup.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p4451
//match_regex: ", line 38: (?:catastrophic )?error
//match_regex: ", line 39: (?:catastrophic )?error
//match_regex: ", line 40: (?:catastrophic )?error
//match_regex: ", line 41: (?:catastrophic )?error
//match_regex: ", line 46: (?:catastrophic )?error
//match_regex: ", line 53: (?:catastrophic )?error
//match_regex: ", line 63: (?:catastrophic )?error
//match_regex: ", line 72: (?:catastrophic )?error
//match_regex: ", line 78: (?:catastrophic )?error
//match_regex: ", line 84: (?:catastrophic )?error
//match_regex: ", line 90: (?:catastrophic )?error
//match_regex: ", line 111: (?:catastrophic )?error
//match_regex: ", line 119: (?:catastrophic )?error
//match_regex: ", line 128: (?:catastrophic )?error
//match_regex: ", line 130: (?:catastrophic )?error
// P4451 [basic.contract.control] (D4451R1): ordinary name lookup for the
// id-expression of an assertion-control-specifier shall find the standard
// library declaration of std::contracts::controls::enforced.  An
// unqualified `enforced' that finds nothing, or that finds a parameter, a
// class member declared later (the specifier is a complete-class context,
// also with a using-directive in scope), an enumerator, a namespace-scope
// variable, a function, a type, a block-scope variable or a lambda's
// parameter, or whose lookup is ambiguous, is rejected, once; one found
// through a using-directive or using-declaration naming the standard
// declaration is not.  Under -fcontracts-p4451 alone.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p4451" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

namespace n0
{
  void f () pre<enforced> (true);	// { dg-error "assertion-control-specifier must name .std::contracts::controls::enforced." }
  struct S { void m () pre<enforced> (true); };	// { dg-error "must name .std::contracts::controls::enforced." }
  void g () { contract_assert<enforced> (true); }	// { dg-error "must name .std::contracts::controls::enforced." }
  auto l = [] () pre<enforced> (true) { };	// { dg-error "must name .std::contracts::controls::enforced." }
}

namespace n1
{
  void f (int enforced) pre<enforced> (true);	// { dg-error "must name .std::contracts::controls::enforced." }
}

namespace n2
{
  struct S
  {
    void f () pre<enforced> (true);	// { dg-error "must name .std::contracts::controls::enforced." }
    static constexpr int enforced = 0;
  };
}

namespace n2u
{
  using namespace std::contracts::controls;
  struct S
  {
    void f () pre<enforced> (true);	// { dg-error "must name .std::contracts::controls::enforced." }
    void g () pre<std::contracts::controls::enforced> (true);
    static constexpr int enforced = 0;
  };
}

namespace n3
{
  enum E { enforced };
  void f () pre<enforced> (true);	// { dg-error "must name .std::contracts::controls::enforced." }
}

namespace n4
{
  constexpr int enforced = 0;
  void f () pre<enforced> (true);	// { dg-error "must name .std::contracts::controls::enforced." }
}

namespace n4f
{
  void enforced ();
  void f () pre<enforced> (true);	// { dg-error "must name .std::contracts::controls::enforced." }
}

namespace n4t
{
  struct enforced { };
  void f () pre<enforced> (true);	// { dg-error "must name .std::contracts::controls::enforced." }
}

namespace n5
{
  using namespace std::contracts::controls;
  void f () pre<enforced> (true);
}

namespace n6
{
  using std::contracts::controls::enforced;
  void f () pre<enforced> (true);
}

namespace n7
{
  namespace a { constexpr int enforced = 0; }
  namespace b { constexpr int enforced = 1; }
  using namespace a;
  using namespace b;
  void f () pre<enforced> (true);	// { dg-error "must name .std::contracts::controls::enforced." }
}

namespace n7s
{
  namespace a { constexpr int enforced = 0; }
  using namespace a;
  using namespace std::contracts::controls;
  void f () pre<enforced> (true);	// { dg-error "must name .std::contracts::controls::enforced." }
}

namespace n8
{
  using namespace std::contracts::controls;
  void g (int x)
  {
    int enforced = x;
    contract_assert<enforced> (x > 0);	// { dg-error "must name .std::contracts::controls::enforced." }
  }
  auto l = [] (int enforced) pre<enforced> (true) { };	// { dg-error "must name .std::contracts::controls::enforced." }
}
