//remark: imported from gcc:dtor-redecl-contracts-not-compared.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 16: (?:catastrophic )?error
//match_regex: ", line 19: (?:catastrophic )?error
// A non-first declaration of a destructor may not add or change contracts
// ([dcl.contract.func]).
//
// Mirror: clang/test/Contracts/dtor-redecl-contracts-not-compared.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

struct B { ~B (); };
B::~B () pre (true) {}		// { dg-error "adds contracts" }

struct C { ~C () pre (true); };
C::~C () pre (false) {}		// { dg-error "mismatched contract condition" }
