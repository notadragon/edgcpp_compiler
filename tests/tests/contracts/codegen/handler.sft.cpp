//remark:contracts: a user-defined violation handler sees the violation's properties
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contract_evaluation_semantic=observe
#include <contracts>
#include <cstdio>

void handle_contract_violation(const std::contracts::contract_violation& v) {
  std::printf("kind=%d semantic=%d mode=%d terminating=%d line=%u\n",
              (int)v.kind(), (int)v.semantic(), (int)v.detection_mode(),
              (int)v.is_terminating(), (unsigned)v.location().line());
  std::printf("  comment: %s\n  function: %s\n", v.comment(),
              v.location().function_name());
}

namespace n {
int f(int x)
  pre(x >
      0)
  post(r: r != 1)
{
  return x;
}
}  // namespace n

int main() {
  n::f(0);
  n::f(1);
  contract_assert(sizeof(int) == 0);
}
