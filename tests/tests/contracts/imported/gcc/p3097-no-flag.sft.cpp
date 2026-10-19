//remark: imported from gcc:p3097-no-flag.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 12: (?:catastrophic )?error
//match_regex: ", line 16: (?:catastrophic )?error
// P3097: Verify that contracts on virtual functions are rejected without flag.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

struct Base {
  virtual void f() pre(true); // { dg-error "contracts cannot be added to virtual functions" }
};

struct Child : Base {
  void f() override pre(true); // { dg-error "contracts cannot be added to virtual functions" }
};
