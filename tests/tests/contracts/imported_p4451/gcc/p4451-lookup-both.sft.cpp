//remark: imported from gcc:p4451-lookup-both.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400 --contracts_p4451
//match_regex: ", line 38: (?:catastrophic )?error
//match_regex: ", line 45: (?:catastrophic )?error
//match_regex: ", line 55: (?:catastrophic )?error
//match_regex: ", line 64: (?:catastrophic )?error
//match_regex: ", line 70: (?:catastrophic )?error
//match_regex: ", line 76: (?:catastrophic )?error
//match_regex: ", line 82: (?:catastrophic )?error
//match_regex: ", line 103: (?:catastrophic )?error
//match_regex: ", line 111: (?:catastrophic )?error
//match_regex: ", line 120: (?:catastrophic )?error
//match_regex: ", line 122: (?:catastrophic )?error
// p4451-lookup.C under -fcontracts-p3400 and -fcontracts-p4451
// (the P3400 label): the P4451 checks step aside and the specifier is
// a label expression.  What P4451 rejects is rejected by P3400's own
// checks, or accepted where P3400 finds a label (<contracts>'s
// contract-control using-directive makes an unqualified `enforced'
// find the standard one).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400 -fcontracts-p4451" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

namespace n0
{
  void f () pre<enforced> (true);
  struct S { void m () pre<enforced> (true); };
  void g () { contract_assert<enforced> (true); }
  auto l = [] () pre<enforced> (true) { };
}

namespace n1
{
  void f (int enforced) pre<enforced> (true);	// { dg-error "(must be a class type|is not a constant expression|is ambiguous|not usable in a constant expression|use of parameter outside function body|expected primary-expression)" }
}

namespace n2
{
  struct S
  {
    void f () pre<enforced> (true);	// { dg-error "(must be a class type|is not a constant expression|is ambiguous|not usable in a constant expression|use of parameter outside function body|expected primary-expression)" }
    static constexpr int enforced = 0;
  };
}

namespace n2u
{
  using namespace std::contracts::controls;
  struct S
  {
    void f () pre<enforced> (true);	// { dg-error "(must be a class type|is not a constant expression|is ambiguous|not usable in a constant expression|use of parameter outside function body|expected primary-expression)" }
    void g () pre<std::contracts::controls::enforced> (true);
    static constexpr int enforced = 0;
  };
}

namespace n3
{
  enum E { enforced };
  void f () pre<enforced> (true);	// { dg-error "(must be a class type|is not a constant expression|is ambiguous|not usable in a constant expression|use of parameter outside function body|expected primary-expression)" }
}

namespace n4
{
  constexpr int enforced = 0;
  void f () pre<enforced> (true);	// { dg-error "(must be a class type|is not a constant expression|is ambiguous|not usable in a constant expression|use of parameter outside function body|expected primary-expression)" }
}

namespace n4f
{
  void enforced ();
  void f () pre<enforced> (true);	// { dg-error "(must be a class type|is not a constant expression|is ambiguous|not usable in a constant expression|use of parameter outside function body|expected primary-expression)" }
}

namespace n4t
{
  struct enforced { };
  void f () pre<enforced> (true);	// { dg-error "(must be a class type|is not a constant expression|is ambiguous|not usable in a constant expression|use of parameter outside function body|expected primary-expression)" }
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
  void f () pre<enforced> (true);	// { dg-error "(must be a class type|is not a constant expression|is ambiguous|not usable in a constant expression|use of parameter outside function body|expected primary-expression)" }
}

namespace n7s
{
  namespace a { constexpr int enforced = 0; }
  using namespace a;
  using namespace std::contracts::controls;
  void f () pre<enforced> (true);	// { dg-error "(must be a class type|is not a constant expression|is ambiguous|not usable in a constant expression|use of parameter outside function body|expected primary-expression)" }
}

namespace n8
{
  using namespace std::contracts::controls;
  void g (int x)
  {
    int enforced = x;
    contract_assert<enforced> (x > 0);	// { dg-error "(must be a class type|is not a constant expression|is ambiguous|not usable in a constant expression|use of parameter outside function body|expected primary-expression)" }
  }
  auto l = [] (int enforced) pre<enforced> (true) { };	// { dg-error "(must be a class type|is not a constant expression|is ambiguous|not usable in a constant expression|use of parameter outside function body|expected primary-expression)" }
}
