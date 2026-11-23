//remark:contracts: the checks generated for the C-generating back end follow the configuration: the postcondition is ignored, so it gets no check
//require:DO_IL_LOWERING 1
//type:fp
//options_all:--no_il_lowering --il_display --contract_evaluation_semantic=quick_enforce '--contract_configuration=[{"match":{"kind":"post"},"output":{"semantic":"ignore"}}]'
//match_regex:^compiler_generated:\s+TRUE\nis_contract_check:\s+TRUE\nkind:\s+stmk_if
// No compiler-generated variable holds the returned value (as it would for a
// checked postcondition).
int f(const int x) pre(x > 0) post(r: r > x) { return x + 1; }
