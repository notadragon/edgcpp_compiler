//remark: imported from gcc:p3400-control-expr.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400
// P3400: contract_control(expr) enables contract-control lookup outside assertions.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400" }

namespace my_labels {
  struct my_label_t {
    using assertion_control_object = my_label_t;
  };
  constexpr my_label_t my_label{};
}

using contract_control namespace my_labels;

// contract_control(expr) uses the augmented lookup.
constexpr auto label_copy = contract_control(my_label);

// Can use the result in an assertion-control expression.
void f(int x)
  pre<label_copy>(x > 0)
{
}

// Also works inline.
void g(int x)
  pre<contract_control(my_label)>(x > 0)
{
}
