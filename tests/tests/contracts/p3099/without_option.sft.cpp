//remark:contracts: without P3099 a contract assertion cannot have a diagnostic message
//type:fn
//options:--no_contracts_p3099:--contracts_p3850 --no_contracts_p3099
//match_regex:line 6: error
void f(int x) {
  contract_assert(x > 0, "m");
}
