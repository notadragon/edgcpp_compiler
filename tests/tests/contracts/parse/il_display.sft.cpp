//remark:contracts: contract specifiers and contract_assert in the IL
//require:DO_IL_LOWERING 1
//type:fp
//options_all:--no_il_lowering --il_display --contract_evaluation_semantic=ignore
//match_regex:^file-scope contract-specifier@X+\nnext:\s+file-scope contract-specifier@X+\nkind:\s+ctk_pre
//match_regex:^file-scope contract-specifier@X+\nnext:\s+NULL\nkind:\s+ctk_post\npredicate:\s+file-scope expr-node@X+\nresult_name:\s+file-scope variable@X+: r
//match_regex:^kind:\s+enk_param_ref\nparam_ref.param_num:\s+1
//match_regex:^contract_specifiers:\s+file-scope contract-specifier@X+
//match_regex:^kind:\s+stmk_contract_assert\ncontract_assert:\s+\S+ contract-specifier@X+
//match_regex:^kind:\s+ctk_assert
//match_regex:^comment:\s+\S+ other-text@X+: "r > x"
int f(const int x) pre(x > 0) post(r: r > x) { return x + 1; }
void g(int z) {
  contract_assert(z > 0);
}
