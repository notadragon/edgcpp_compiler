//remark:contracts: P4405: constant evaluation is never analyzed: a prove or diagnose it reaches (the command line, a configuration entry without a "constexpr" key) is evaluated as enforce or observe there, so a false predicate makes the expression not constant under prove and is warned about under diagnose; --contract_analysis_semantic does not apply to it (the default semantic does); an entry with "constexpr": true selecting prove or diagnose is a configuration error (see p4405/analysis_config)
//type:fn
//options_sep:!
//options:--contracts_p4405 --contract_evaluation_semantic=prove!--contracts_p4405 '--contract_configuration=[{"match":{"constexpr":false},"output":{"semantic":"ignore"}},{"output":{"semantic":"prove"}}]'!--contracts_p4405 --contract_evaluation_semantic=diagnose;fp!--contracts_p4405 --contract_analysis_semantic=prove
//match_regex:line ([0-9]+): (error|warning): (contract predicate is false in constant expression|static assertion failed|expression must have a constant value)
// (No system headers: also runs under the C-generating back end.)  Under
// prove, and under the default with the analysis semantic prove, the
// precondition fails the constant evaluation (enforce); under diagnose each
// assertion warns and the evaluation continues (observe).  No assertion is
// analyzed: f is never called at run time.
constexpr int f(int x) pre (x > 0) post (r: r > 0) {
  contract_assert(x > 0);
  return x;
}
static_assert(f(-1) == -1);
