//remark: imported from gcc:p3400-label-bad-type.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400
//match_regex: ", line 21: (?:catastrophic )?error
//match_regex: ", line 26: (?:catastrophic )?error
//match_regex: ", line 31: (?:catastrophic )?error
// P3400: Error when label type lacks assertion_control_object.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400" }

struct not_a_label {};
constexpr not_a_label bad_label;

struct also_bad {
  int assertion_control_object = 0;  // not a type
};
constexpr also_bad bad_label2{};

void f(int x)
  pre<bad_label>(x > 0)  // { dg-error "assertion_control_object" }
{
}

void g(int x)
  pre<bad_label2>(x > 0)  // { dg-error "assertion_control_object" }
{
}

void h() {
  contract_assert<42>(true);  // { dg-error "class type" }
}
