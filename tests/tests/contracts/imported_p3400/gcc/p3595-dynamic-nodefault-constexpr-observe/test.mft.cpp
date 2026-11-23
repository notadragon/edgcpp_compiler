//remark: imported from gcc:p3595-dynamic-nodefault-constexpr-observe.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-dynamic-nodefault.json
//options: --c++26 --contracts --contracts_p3400 --contract_evaluation_semantic=observe --contract_configuration_file=p3595-dynamic-nodefault.json
// P3595: a "dynamic" entry with no "semantic" (the shape
// p3595-dynamic-nodefault.C runs) never matches during constant evaluation
// (design section 3, half B), and when it is the only entry nothing else
// does either, so constant evaluation uses the command-line default:
// observe here, given -fcontract-evaluation-semantic=observe (a warning,
// and the call is a constant).  p3595-dynamic-constexpr-skip.C covers the
// fall-through to a later entry (GCC-201).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontract-evaluation-semantic=observe" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p3595-dynamic-nodefault.json" }

#include <contracts>

using std::contracts::evaluation_semantic;

evaluation_semantic
p3595_sel_nd ()
{
  return evaluation_semantic::observe;
}

constexpr int g (int x) pre (x > 0) { return x; } // { dg-warning "contract predicate is false in constant expression" }
constexpr int bad = g (-1);
constexpr int ok = g (1);
static_assert (bad == -1 && ok == 1);
