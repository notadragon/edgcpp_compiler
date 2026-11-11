//remark:contracts: through edg-gxx, --contracts_p3100 reaches g++, whose implicit contract assertions check the generated C++ (the front end makes none), configured by the "implicit" kind
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_sep:!
//options:--contracts_p3100 '--contract_configuration=[{"match":{"kind":"implicit"},"output":{"semantic":"observe"}}]'
//match_regex:^(violation kind 7|result 0|end)$
#include <contracts>
#include <cstdio>
void handle_contract_violation(const std::contracts::contract_violation &v) {
  std::printf("violation kind %d\n", (int)v.kind());
}
int divide(int a, int b) { return a / b; }
volatile int zero = 0;
int main() {
  std::printf("result %d\n", divide(1, zero));
  std::puts("end");
}
