//remark:contracts: a NUL character ("\u0000") in any string field of an inline configuration entry is a command-line error, not cut at the NUL, under both back ends
//type:fc
//options_sep:!
//options:'--contract_configuration=[{"match":{"kind":"pre\u0000x"},"output":{"semantic":"ignore"}}]';fc!'--contract_configuration=[{"match":{"group":"g\u0000x"},"output":{"semantic":"ignore"}}]';fc!'--contract_configuration=[{"match":{"namespace":"n\u0000x"},"output":{"semantic":"ignore"}}]';fc!'--contract_configuration=[{"match":{"location":"t.cpp\u0000x"},"output":{"semantic":"ignore"}}]';fc!'--contract_configuration=[{"match":{"caller":{"location":"t.cpp\u0000x"}},"output":{"semantic":"ignore"}}]';fc!'--contract_configuration=[{"match":{"caller":{"namespace":"n\u0000x"}},"output":{"semantic":"ignore"}}]';fc!'--contract_configuration=[{"output":{"semantic":"ignore\u0000x"}}]';fc!'--contract_configuration=[{"output":{"semantic":"ignore","dynamic":{"name":"sel\u0000x"}}}]';fc!'--contract_configuration=[{"output":{"semantic":"ignore","dynamic":{"name":"sel","linkage":"C\u0000x"}}}]';fc
//match_regex:Command-line error: invalid contract configuration \(<command-line>:1:[0-9]+\): "(kind|group|namespace|location|semantic|name|linkage)"( in "(caller|dynamic)")? must not contain a null character$
// notadragon_wg21 DECISIONS.md E193 (1): one case per field -- "kind",
// "group", "namespace", "location", "location" and "namespace" in
// "caller", "semantic", and "name" and "linkage" in "dynamic" -- each
// recorded with the field it names (as in GCC, "linkage" without "in
// "dynamic"").  Through edg-gxx the front end stops on the error.  field_nul_file is the file form; group_nul holds more "group"
// cases.
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/
// p3595-config-field-nul-inline.C (gnu_gcc).

int f(int x) pre(x > 0) { return x; }
