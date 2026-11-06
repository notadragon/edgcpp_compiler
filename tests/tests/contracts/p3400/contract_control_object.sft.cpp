//remark:contracts: P3400 contract_control(e) is an lvalue for a constexpr object, one for each type and value, and is put out for g++ as written, also as the whole initializer of a variable
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contracts_p3400
struct Label { using assertion_control_object = Label; };
struct Valued { using assertion_control_object = Valued; int v; };
namespace L {
  inline constexpr Label lbl{};
  inline constexpr Valued val{3};
}
using contract_control namespace L;
constexpr auto x = contract_control(lbl);       // An empty class.
constexpr auto y = contract_control(val);
constexpr const Valued *p = &contract_control(val);
static_assert(p == &contract_control(Valued{3}));
static_assert(&contract_control(42) != nullptr);
int main() { return y.v + p->v == 6 ? 0 : 1; }
