//remark:contracts: in constant evaluation noexcept_enforce is enforce (an error) and noexcept_observe is observe (a warning), from a configuration (in the C-generating configuration, an entry for constant evaluation only)
//type:fn
//options_sep:!
//options:--contracts_p4298 '--contract_configuration=[{"match":{"constexpr":true},"output":{"semantic":"noexcept_enforce"}},{"output":{"semantic":"ignore"}}]';fn!--contracts_p4298 '--contract_configuration=[{"match":{"constexpr":true},"output":{"semantic":"noexcept_observe"}},{"output":{"semantic":"ignore"}}]';fp
//match_regex:line 10: (error|warning): contract predicate is false in constant expression
// (No system headers: also runs under the C-generating back end.)
// Constant evaluation is the front end's in both configurations; a run-time
// check is ignored (the C-generating back end supports no other semantic
// for it here).
constexpr int f(int x) pre (x > 0) { return x; }
constexpr int v = f(0);
