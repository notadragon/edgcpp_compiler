//remark:contracts: a malformed configuration is a command-line error, at its position in the JSON text
//type:fc
//options_sep:!
//options:'--contract_configuration=[{"output":{"semantic":"bogus"}}]';fc!'--contract_configuration=[1]';fc!'--contract_configuration=[{"match":{"kind":"pre"}}]';fc!'--contract_configuration=[{"output":{"dynamic":{"name":"a::::b"}}}]';fc!'--contract_configuration=[{"output":{"semantic":"ignore"},"match":{"location":"f.cpp:3-1"}}]';fc!--contract_group_evaluation_semantic=.a:ignore;fc!--contract_configuration_file=no_such_file.json;fc
//match_regex:Command-line error: (invalid contract configuration|cannot read contract configuration file)
int x;
