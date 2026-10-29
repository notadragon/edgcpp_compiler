//remark:contracts: P4405's "analysis" match key, as GCC's (p4405-analysis-config-errors): "analysis": false is the same as leaving the key out, so it may appear with "caller"; an analysis entry with "constexpr": true is accepted and never matches (constant evaluation is never analyzed); an analysis entry for preconditions with ignore makes their analysis evaluations ignore, the later catch-all for the others not applying to them
//type:fp
//options_sep:!
//options:--contracts_p4405 --contract_evaluation_semantic=ignore '--contract_configuration=[{"match":{"analysis":false,"caller":true},"output":{"semantic":"ignore"}},{"match":{"analysis":true,"constexpr":true},"output":{"semantic":"prove"}},{"match":{"analysis":true,"kind":"pre"},"output":{"semantic":"ignore"}},{"match":{"analysis":true},"output":{"semantic":"prove"}}]'
// No diagnostic: f's precondition is analyzed with ignore at its call
// (with the C-generating back end, the front end would otherwise report
// an error there).
int f(int x) pre (x > 0) { return x; }
int g() { return f(0); }
