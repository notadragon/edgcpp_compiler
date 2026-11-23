//remark: imported from gcc:p4299-pre-post-keywords.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4299 --contract_evaluation_semantic=enforce
//match_regex: ", line 21: (?:catastrophic )?error
//match_regex: ", line 22: (?:catastrophic )?error
//match_regex: ", line 23: (?:catastrophic )?error
//match_regex: ", line 24: (?:catastrophic )?error
// With -fcontracts-p4299 in C++, _Pre and _Post are keywords
// (notadragon_wg21 DECISIONS.md E28), as _ContractAssert is, and introduce
// a precondition and a postcondition; pre and post stay ordinary
// identifiers (GCC-637).
//
// Mirror: clang/test/Contracts/p4299-pre-post-keywords.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4299 -fcontract-evaluation-semantic=enforce" }

namespace a
{
  int _Pre = 1;   // { dg-error "expected" }
  int _Post = 2;  // { dg-error "expected" }
  int _ContractAssert = 3;  // { dg-error "expected" }
  int g () { return _Pre; }  // { dg-error "expected" }
}

namespace b
{
  int pre = 4;    // OK, an ordinary identifier
  int post = 5;   // OK

  int f (int x) _Pre (x > 0) _Post (r: r > 0)
  {
    _ContractAssert (x > 0);
    return pre + post;
  }
}
