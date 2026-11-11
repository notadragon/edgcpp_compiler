//remark:contracts: with P4405 a terminating contract assertion (P4451) allows prove too, as P3400's labels::terminating does (label_allowed_proving_semantics 1)
//require:DO_IL_LOWERING 1
//type:fp
//options_all:--contracts_p4451 --contracts_p4405 --no_il_lowering --il_display
//filter:grep -aE -A1 '^(is_terminating|label_allowed|label:)'
//match_regex:^is_terminating:\s+TRUE\nlabel_allowed_semantics:\s+24\nlabel_allowed_proving_semantics:\s+1\n
namespace std::contracts::labels {
  inline constexpr struct terminating_t {} terminating{};
}

int f(int x) pre<terminating>(x > 0) { return x; }
