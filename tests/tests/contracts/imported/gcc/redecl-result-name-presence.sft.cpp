//remark: imported from gcc:redecl-result-name-presence.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 18: (?:catastrophic )?error
//match_regex: ", line 20: (?:catastrophic )?error
// Two declarations of a function whose postconditions differ only in the
// presence of a result-name-introducer are ill-formed ([dcl.contract.func],
// the draft's own example), in either order.
//
// Mirror: clang/test/Contracts/redecl-result-name-presence.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

bool b1;
int g () post (r: b1);
int g () post (b1);		// { dg-error "mismatch" }
int h () post (b1);
int h () post (r: b1);		// { dg-error "mismatch" }
