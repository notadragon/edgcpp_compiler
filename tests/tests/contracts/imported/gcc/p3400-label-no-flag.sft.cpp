//remark: imported from gcc:p3400-label-no-flag.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26
//match_regex: ", line 15: (?:catastrophic )?error
// P3400: Label syntax rejected without -fcontracts-p3400.
// { dg-do compile { target c++26 } }

struct my_label_t {
  using assertion_control_object = my_label_t;
};
constexpr my_label_t my_label{};

void f(int x)
  pre<my_label>(x > 0)  // { dg-error "assertion-control labels require" }
{
}
