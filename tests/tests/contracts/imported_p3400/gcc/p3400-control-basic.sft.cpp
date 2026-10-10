//remark: imported from gcc:p3400-control-basic.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400
// P3400: using contract_control namespace enables unqualified label access.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400" }

namespace my_labels {
  struct my_label_t {
    using assertion_control_object = my_label_t;
  };
  constexpr my_label_t my_label{};
}

using contract_control namespace my_labels;

// Unqualified 'my_label' should be found in assertion-control expressions.
void f(int x)
  pre<my_label>(x > 0)
{
}

// But NOT in regular code:
// auto bad = my_label;  // would be an error without regular 'using namespace'
