//remark:contracts: P3400 a label or contract_control operand that starts with a user-defined literal finds its literal operator through <contracts>' contract_control using-directive
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options_all:--contracts_p3400
#include <contracts>
void f(int x) pre<"quietgroup"group>(x > 0) {}
void g() { contract_assert<"q"group>(true); }
constexpr auto z = contract_control("w"group);
