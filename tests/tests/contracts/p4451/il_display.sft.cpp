//remark:contracts: a terminating contract assertion (P4451) is marked is_terminating, has no label, and allows the semantics of P3400's labels::terminating: enforce and quick_enforce (label_allowed_semantics 24, as for that label)
//require:DO_IL_LOWERING 1
//type:fp
//options_all:--contracts_p4451 --no_il_lowering --il_display
//filter:grep -aE -A1 '^(is_terminating|label_allowed|label:)'
//match_regex:^is_terminating:\s+TRUE\nlabel_allowed_semantics:\s+24\n
namespace std::contracts::labels {
  inline constexpr struct terminating_t {} terminating{};
}

int f(int x) pre<terminating>(x > 0) { return x; }
