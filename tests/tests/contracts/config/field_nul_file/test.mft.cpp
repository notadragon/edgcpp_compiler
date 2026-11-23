//remark:contracts: a NUL character ("\u0000") in any string field of a configuration file entry is a command-line error, not cut at the NUL, for the front end and a full compilation, under both back ends
//type:fc
//source_files:kind.json group.json namespace.json location.json caller_location.json caller_namespace.json semantic.json dynamic_name.json dynamic_linkage.json
//options:--contract_configuration_file=kind.json;fc:--contract_configuration_file=group.json;fc:--contract_configuration_file=namespace.json;fc:--contract_configuration_file=location.json;fc:--contract_configuration_file=caller_location.json;fc:--contract_configuration_file=caller_namespace.json;fc:--contract_configuration_file=semantic.json;fc:--contract_configuration_file=dynamic_name.json;fc:--contract_configuration_file=dynamic_linkage.json;fc:--contract_configuration_file=kind.json;cc
//match_regex:Command-line error: invalid contract configuration \([a-z_]+\.json:2:[0-9]+\): "(kind|group|namespace|location|semantic|name|linkage)"( in "(caller|dynamic)")? must not contain a null character$
// notadragon_wg21 DECISIONS.md E193 (1): each file holds one entry with a
// NUL in a different field, since the first error stops the compilation.
// Through edg-gxx the front end stops on the error.  field_nul.sft.cpp is
// the inline form; group_nul_file holds the "group" field alone.
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/p3595-config-field-nul.C
// (gnu_gcc).

int f(int x) pre(x > 0) { return x; }
