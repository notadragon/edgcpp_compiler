//remark:contracts: P3400 an ordinary using-directive for std::contracts::labels makes its names visible everywhere, although <contracts> already nominates it with a contract_control using-directive
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contracts_p3400 --contract_evaluation_semantic=observe
#include <contracts>
using namespace std::contracts::labels;

constexpr auto safety = "safety"group;           // OK, found by ordinary lookup
constexpr auto plain = empty_label;              // OK
static_assert(&contract_control(safety) == &contract_control(safety));

static int violations = 0;
void handle_contract_violation(const std::contracts::contract_violation &)
{
  ++violations;
}

void f(int x) pre<safety>(x > 0) pre<plain>(x > 1) {}

int main()
{
  f(1);
  return violations == 1 ? 0 : 1;
}
