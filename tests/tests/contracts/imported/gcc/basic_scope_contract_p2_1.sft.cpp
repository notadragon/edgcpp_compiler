//remark: imported from gcc:basic.scope.contract.p2.1.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
//match_regex: ", line 15: (?:catastrophic )?error
// N5008 :
// basic.scope.contract/p2.1
// If a result-name-introducer (9.4.2) that is not name-independent (6.4.1) and whose enclosing postcondition
// assertion is associated with a function F potentially conflicts with a declaration whose target scope is
//	- the function parameter scope of F or
// { dg-do compile { target c++23 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=observe " }

int bad_mr_shadow (int r)
  post (r: r > 5) // { dg-error "contract postcondition result name shadows a function parameter" }
  { return r + 1; }
