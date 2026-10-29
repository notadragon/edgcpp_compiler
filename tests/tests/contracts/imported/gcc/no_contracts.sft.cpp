//remark: imported from gcc:no_contracts.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --no_contracts --c++26
//match_regex: ", line 11: (?:catastrophic )?error
//match_regex: ", line 12: (?:catastrophic )?error
// { dg-options "-fno-contracts -std=c++26" }

void f()
{
    contract_assert(false); // { dg-error "'contract_assert' is only available with '-fcontracts'" }
    __contract_assert(false); // { dg-error "'__contract_assert' is only available with '-fcontracts'" }
}
