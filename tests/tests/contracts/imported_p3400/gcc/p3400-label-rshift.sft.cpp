//remark: imported from gcc:p3400-label-rshift.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400
// P3400: Variable template label with trailing >> (rshift split).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400" }

struct empty_label_t {
  using assertion_control_object = empty_label_t;
};

template<typename T>
constexpr empty_label_t typed_label{};

void f(int x)
  pre<typed_label<int>>(x > 0)
{
}

int g(int x)
  post<typed_label<int>>(r: r >= 0)
{
  return x;
}

void h() {
  contract_assert<typed_label<int>>(true);
}
