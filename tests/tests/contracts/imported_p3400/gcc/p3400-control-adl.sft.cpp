//remark: imported from gcc:p3400-control-adl.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400
// P3400: Free-function operators found through contract_control using directives.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400" }

namespace label_ops {
  struct base_label_t {
    using assertion_control_object = base_label_t;
    int tag;
  };
  constexpr base_label_t alpha{0};
  constexpr base_label_t beta{1};

  struct combined_label_t {
    using assertion_control_object = combined_label_t;
    base_label_t left;
    base_label_t right;
  };

  // Free-function operator| that combines labels.
  constexpr combined_label_t operator|(base_label_t a, base_label_t b) {
    return combined_label_t{a, b};
  }
}

using contract_control namespace label_ops;

// The operator| should be found via contract_control lookup
// when used within an assertion-control expression.
void f(int x)
  pre<(alpha | beta)>(x > 0)
{
}

// Also in contract_control(expr).
constexpr auto combined = contract_control(alpha | beta);

void g(int x)
  pre<combined>(x > 0)
{
}

// Verify the type is correct.
static_assert(__is_same(decltype(combined), const label_ops::combined_label_t));
