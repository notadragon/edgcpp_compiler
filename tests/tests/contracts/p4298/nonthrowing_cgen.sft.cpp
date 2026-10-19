//remark:contracts: without P4298 a noexcept semantic becomes observe or enforce, which the C-generating back end rejects: a command-line error, and an error from a configuration entry outside constant evaluation
//require:DO_IL_LOWERING 1
//type:fc
//options_sep:!
//options:--contract_evaluation_semantic=noexcept_enforce;fc!--contract_evaluation_semantic=noexcept_observe;fc!'--contract_configuration=[{"output":{"semantic":"noexcept_enforce"}}]';fc
//match_regex:Command-line error: (this configuration supports only the ignore, quick_enforce, noexcept_enforce, noexcept_observe and assume contract evaluation semantics: (enforce|observe)|invalid contract configuration \([^)]*\): this configuration supports only the ignore, quick_enforce, noexcept_enforce, noexcept_observe and assume contract evaluation semantics)
int x;
