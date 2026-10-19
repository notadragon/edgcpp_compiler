//remark:contracts: the checks of contract assertions in the IL
//require:DO_IL_LOWERING 1
//type:fp
//options_all:--no_il_lowering --il_display --contract_evaluation_semantic=quick_enforce
//match_regex:^is_contract_result:\s+TRUE\ninit_kind:\s+initk_none
//match_regex:^contract_prologue:\s+func-scope statement@X+\ncontract_epilogue:\s+func-scope statement@X+\ncontract_result_variable:\s+func-scope variable@X+
//match_regex:^compiler_generated:\s+TRUE\nis_contract_check:\s+TRUE\nkind:\s+stmk_if\nexpr:\s+func-scope expr-node@X+\nthen_statement:
//match_regex:^kind:\s+ctk_assert\npredicate:\s+NULL
// Each check is a compiler-generated "if" calling __builtin_trap, marked as
// a contract check (which constant evaluation skips).  The checks of the
// precondition and postcondition are on the function scope, for IL lowering
// to put into the lowered function, and the postcondition names as the
// result a compiler-generated variable, which lowering sets; the
// contract_assert's predicate moves into its check.
int f(const int x) pre(x > 0) post(r: r > x) { return x + 1; }
void g(int z) {
  contract_assert(z > 0);
}
