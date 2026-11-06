//remark:contracts: P4405's "analysis" match key, as GCC's (p4405-analysis-config-errors, p4405-constexpr): it must be a boolean, cannot appear with "caller", and its entry's output must be ignore, diagnose or prove with no "dynamic"; an entry with "constexpr": true cannot select prove or diagnose (constant evaluation is never analyzed); --contract_analysis_semantic takes ignore, diagnose or prove, and without --contracts_p4405 is ignore with a warning (each configuration error ends the compilation; see p4405/analysis_config_ok for the entries that are accepted)
//type:fc
//options_sep:!
//options:--contracts_p4405 '--contract_configuration=[{"match":{"analysis":"yes"},"output":{"semantic":"prove"}}]'!--contracts_p4405 '--contract_configuration=[{"match":{"analysis":true,"caller":false},"output":{"semantic":"prove"}}]'!--contracts_p4405 '--contract_configuration=[{"match":{"analysis":true},"output":{"semantic":"prove","dynamic":{"name":"sel"}}}]'!--contracts_p4405 '--contract_configuration=[{"match":{"analysis":true},"output":{"semantic":"enforce"}}]'!--contracts_p4405 '--contract_configuration=[{"match":{"analysis":true},"output":{"semantic":"assume"}}]'!--contracts_p4405 '--contract_configuration=[{"match":{"constexpr":true,"kind":"pre"},"output":{"semantic":"prove"}}]'!--contracts_p4405 '--contract_configuration=[{"match":{"constexpr":true},"output":{"semantic":"diagnose"}}]'!--contracts_p4405 --contract_analysis_semantic=enforce!--contracts --contract_analysis_semantic=prove --contract_evaluation_semantic=ignore;fp
//match_regex:^(Command-line (error|warning): .*)$
// Under the last options nothing else is diagnosed: f's precondition is
// analyzed with ignore at its call (P4405 is off).
int f(int x) pre (x > 0) { return x; }
int g() { return f(0); }
