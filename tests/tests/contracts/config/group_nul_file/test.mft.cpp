//remark:contracts: a "group" in a configuration file entry holding a NUL character ("\u0000") is a command-line error, not cut at the NUL, for the front end and a full compilation, under both back ends
//type:fc
//source_files:group_nul.json
//options:--contract_configuration_file=group_nul.json;fc:--contract_configuration_file=group_nul.json;cc
//match_regex:Command-line error: invalid contract configuration \(group_nul\.json:2:[0-9]+\): "group" must not contain a null character$
// notadragon_wg21 DECISIONS.md E192 (1)(a).  Through edg-gxx the front end
// stops on the error, and g++, which gives the same error, adds nothing.
// group_nul.sft.cpp is the inline form.
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/p3595-config-group-nul.C
// (gnu_gcc).

int f(int x) pre(x > 0) { return x; }
