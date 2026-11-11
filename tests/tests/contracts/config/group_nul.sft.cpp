//remark:contracts: a "group" in an inline configuration entry holding a NUL character ("\u0000") is a command-line error, not cut at the NUL, for the front end and a full compilation, under both back ends
//type:fc
//options_sep:!
//options:'--contract_configuration=[{"match":{"group":"safety\u0000x"},"output":{"semantic":"ignore"}}]';fc!'--contract_configuration=[{"match":{"group":"safety\u0000x"},"output":{"semantic":"ignore"}}]';cc!'--contract_configuration=[{"match":{"group":"safety\u0000"},"output":{"semantic":"ignore"}}]';fc!'--contract_configuration=[{"match":{"group":"\u0000"},"output":{"semantic":"ignore"}}]';fc
//match_regex:Command-line error: invalid contract configuration \(<command-line>:1:[0-9]+\): "group" must not contain a null character$
// notadragon_wg21 DECISIONS.md E192 (1)(a).  Through edg-gxx the front end
// stops on the error, and g++, which gives the same error, adds nothing.
// group_nul_file is the file form.
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/
// p3595-config-group-nul-inline.C (gnu_gcc).

int f(int x) pre(x > 0) { return x; }
