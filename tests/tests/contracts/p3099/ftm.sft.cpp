//remark:contracts: P3099 predefines __cpp_contracts_message, with contracts
//type:fp
//options:--contracts_p3099:--contracts_p3850:--no_contracts_p3099:--contracts_p3099 --no_contracts
#if TEST_NUMBER <= 2
#if __cpp_contracts_message <= 0
#error __cpp_contracts_message
#endif
#elif defined(__cpp_contracts_message)
#error __cpp_contracts_message defined
#endif
int x;
