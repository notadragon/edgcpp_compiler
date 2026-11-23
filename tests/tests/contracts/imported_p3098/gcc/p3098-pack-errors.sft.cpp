//remark: imported from gcc:p3098-pack-errors.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098
//match_regex: ", line 13: (?:catastrophic )?error
//match_regex: ", line 17: (?:catastrophic )?error
// P3098: Pack capture syntax -- error cases.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098" }

// Pack expansion on non-pack parameter -- error.
void f1(int i)
  post [i...] (true);  // { dg-error "pack expansion.*non-pack" }

// Init-capture pack with no packs in expression -- error.
void f2(int i)
  post [...old = i] (true);  // { dg-error "no parameter packs" }
