//remark: imported from gcc:p3400-label-basic.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400
// P3400: Basic label syntax parsing.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400" }

struct empty_label_t {
  using assertion_control_object = empty_label_t;
};
constexpr empty_label_t empty_label;

void f(int x)
  pre<empty_label>(x > 0)
{
}

int g(int x)
  post<empty_label>(r: r >= 0)
{
  return x;
}

void h() {
  contract_assert<empty_label>(true);
}
