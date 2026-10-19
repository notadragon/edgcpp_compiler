//remark:contracts: in the C-generating configuration an entry may give only ignore or quick_enforce outside constant evaluation, and no dynamic output
//require:DO_IL_LOWERING 1
//type:fc
//options_sep:!
//options:'--contract_configuration=[{"output":{"semantic":"enforce"}}]';fc!'--contract_configuration=[{"output":{"semantic":"observe","dynamic":{"name":"sel"}}}]';fc!--contract_group_evaluation_semantic=a:observe;fc
//match_regex:Command-line error: invalid contract configuration \([^)]*\): this configuration (supports only the ignore, quick_enforce, noexcept_enforce, noexcept_observe and assume contract evaluation semantics|does not support a "dynamic" output)
int x;
