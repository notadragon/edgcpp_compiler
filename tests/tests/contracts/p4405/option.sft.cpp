//remark:contracts: P4405: --contracts_p4405 (implied by --contracts_p3850) predefines __cpp_contracts_prove; without it prove and diagnose are ignore, with a warning, whether named on the command line, in a configuration or as a group semantic; --[no_]contracts_prove_fold is accepted on either side of it
//type:fp
//options_sep:!
//options:--contracts_p4405 -DWANT=1!--contracts_p3850 -DWANT=1!--contracts -DWANT=0!--contracts_p3850 --no_contracts_p4405 -DWANT=0!--contracts --contract_evaluation_semantic=prove -DWANT=0!--contracts --contract_evaluation_semantic=diagnose -DWANT=0!--contracts '--contract_configuration=[{"match":{"kind":"pre"},"output":{"semantic":"prove"}},{"output":{"semantic":"diagnose"}}]' -DWANT=0!--contracts_p3400 --contract_group_evaluation_semantic=g:prove,h:diagnose -DWANT=0!--contracts_p4405 --no_contracts_prove_fold -DWANT=1!--no_contracts_prove_fold --contracts_p3850 -DWANT=1!--contracts_p4405 --no_contracts_prove_fold --contracts_prove_fold -DWANT=1
//match_regex:^(Command-line warning: --contract_evaluation_semantic=(prove|diagnose) requires --contracts_p4405; ignore is used instead|.*the configuration asks for "(prove|diagnose)", which requires --contracts_p4405; "ignore" is used instead|)$
#if defined(__cpp_contracts_prove) != WANT
#error the macro
#endif
#if WANT && __cpp_contracts_prove <= 0
#error the value
#endif
bool chk(int x);
int f(int x) pre (chk(x)) post (r: chk(r)) { contract_assert(chk(x)); return x; }
