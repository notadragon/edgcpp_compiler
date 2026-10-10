//remark: imported from gcc:default-outofline-adds-contract.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 14: (?:catastrophic )?error
//match_regex: ", line 17: (?:catastrophic )?error
// An out-of-line explicitly defaulted special member whose defaulting
// declaration adds contracts is ill-formed; the predicates are parsed and
// compared before the function is synthesized.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

struct S { S (); };
S::S () pre (true) = default;		// { dg-error "adds contracts" }

struct D { ~D (); };
D::~D () pre (true) = default;		// { dg-error "adds contracts" }
