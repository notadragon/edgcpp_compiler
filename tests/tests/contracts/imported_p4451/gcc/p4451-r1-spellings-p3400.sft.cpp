//remark: imported from gcc:p4451-r1-spellings-p3400.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400 --contract_evaluation_semantic=ignore
//match_regex: ", line 43: (?:catastrophic )?error
//match_regex: ", line 44: (?:catastrophic )?error
//match_regex: ", line 47: (?:catastrophic )?error
//match_regex: ", line 52: (?:catastrophic )?error
//match_regex: ", line 55: (?:catastrophic )?error
//match_regex: ", line 61: (?:catastrophic )?error
//match_regex: ", line 64: (?:catastrophic )?error
//match_regex: ", line 70: (?:catastrophic )?error
//match_regex: ", line 76: (?:catastrophic )?error
//match_regex: ", line 83: (?:catastrophic )?error
//match_regex: ", line 88: (?:catastrophic )?error
//match_regex: ", line 96: (?:catastrophic )?error
//match_regex: ", line 97: (?:catastrophic )?error
//match_regex: ", line 103: (?:catastrophic )?error
//match_regex: ", line 108: (?:catastrophic )?error
//match_regex: ", line 114: (?:catastrophic )?error
//match_regex: ", line 117: (?:catastrophic )?error
//match_regex: ", line 118: (?:catastrophic )?error
// P4451 [basic.contract.control] (D4451R1): the assertion-control-specifier
// is `< id-expression >', and ordinary name lookup for it must find the
// standard library declaration of std::contracts::controls::enforced.  A
// qualified name, one qualified from the global namespace, a name through
// a namespace alias, and an unqualified name found through a
// using-directive (at namespace scope, at block scope, and in a namespace
// nominated by one) or a using-declaration all name it; a member function's
// qualified specifier is unaffected by a later member of the same name
// (complete-class context); a non-dependent specifier in a template.  Each
// makes the assertion terminating: with ignore configured, constant
// evaluation of a false predicate fails.  Under -fcontracts-p3400 alone (the P3400 label).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400 -fcontract-evaluation-semantic=ignore" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

namespace scl = std::contracts::controls;

// R1: qualified, on each assertion kind.
constexpr int r1_pre (int x) pre<std::contracts::controls::enforced> (x > 0) { return x; }	// { dg-error "contract predicate is false" }
constexpr int r1_post (int x) post<std::contracts::controls::enforced> (r : r > 0) { return x; }	// { dg-error "contract predicate is false" }
constexpr int r1_assert (int x)
{
  contract_assert<std::contracts::controls::enforced> (x > 0);	// { dg-error "contract predicate is false" }
  return x;
}

// R2: qualified from the global namespace.
constexpr int r2 (int x) pre<::std::contracts::controls::enforced> (x > 0) { return x; }	// { dg-error "contract predicate is false" }

// R3: a namespace alias.
constexpr int r3 (int x) pre<scl::enforced> (x > 0) { return x; }	// { dg-error "contract predicate is false" }

// R4: unqualified, through a using-directive.
namespace r4ns
{
  using namespace std::contracts::controls;
  constexpr int f (int x) pre<enforced> (x > 0) { return x; }	// { dg-error "contract predicate is false" }
  namespace inner
  {
    constexpr int g (int x) pre<enforced> (x > 0) { return x; }	// { dg-error "contract predicate is false" }
  }
}
constexpr int r4_block (int x)
{
  using namespace std::contracts::controls;
  contract_assert<enforced> (x > 0);	// { dg-error "contract predicate is false" }
  return x;
}
namespace r4part
{
  using namespace std::contracts;
  constexpr int f (int x) pre<controls::enforced> (x > 0) { return x; }	// { dg-error "contract predicate is false" }
}

// R5: unqualified, through a using-declaration.
namespace r5ns
{
  using std::contracts::controls::enforced;
  constexpr int f (int x) post<enforced> (r : r > 0) { return x; }	// { dg-error "contract predicate is false" }
}
constexpr int r5_block (int x)
{
  using std::contracts::controls::enforced;
  contract_assert<enforced> (x > 0);	// { dg-error "contract predicate is false" }
  return x;
}

// R7: a member function; the class's own `enforced', declared later, is
// not what the qualified name finds.
struct R7
{
  static constexpr int f (int x) pre<std::contracts::controls::enforced> (x > 0) { return x; }	// { dg-error "contract predicate is false" }
  static constexpr int g (int x) pre<scl::enforced> (x > 0) { return x; }	// { dg-error "contract predicate is false" }
  static constexpr int enforced = 0;
};

// R8: non-dependent specifiers in templates, instantiated.
template <class T>
constexpr T r8_f (T x) pre<std::contracts::controls::enforced> (x > 0) { return x; }	// { dg-error "contract predicate is false" }
template <class T>
constexpr T r8_assert (T x)
{
  using namespace std::contracts::controls;
  contract_assert<enforced> (x > 0);	// { dg-error "contract predicate is false" }
  return x;
}
template <class T>
struct R8
{
  static constexpr T f (T x) pre<scl::enforced> (x > 0) { return x; }	// { dg-error "contract predicate is false" }
  static constexpr int enforced = 0;
};
constexpr auto r8_lambda = [] (int x) pre<scl::enforced> (x > 0) { return x; };	// { dg-error "contract predicate is false" }
constexpr auto r8_generic = [] (auto x) pre<std::contracts::controls::enforced> (x > 0) { return x; };	// { dg-error "contract predicate is false" }

constexpr int ok = r1_pre (1) + r3 (1) + R7::f (1) + r8_f (1) + R8<int>::f (1);
constexpr int a1 = r1_pre (-1);
constexpr int a2 = r1_post (-1);
constexpr int a3 = r1_assert (-1);
constexpr int a4 = r2 (-1);
constexpr int a5 = r3 (-1);
constexpr int a6 = r4ns::f (-1);
constexpr int a7 = r4ns::inner::g (-1);
constexpr int a8 = r4_block (-1);
constexpr int a9 = r4part::f (-1);
constexpr int a10 = r5ns::f (-1);
constexpr int a11 = r5_block (-1);
constexpr int a12 = R7::f (-1);
constexpr int a13 = R7::g (-1);
constexpr int a14 = r8_f (-1);
constexpr long a15 = r8_assert (-1L);
constexpr int a16 = R8<int>::f (-1);
constexpr int a17 = r8_lambda (-1);
constexpr int a18 = r8_generic (-1);
