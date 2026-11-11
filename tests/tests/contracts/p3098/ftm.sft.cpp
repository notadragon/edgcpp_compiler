//remark:contracts: P3098 predefines __cpp_contracts_postcondition_captures, with contracts (with a final --no_contracts, --contracts_p3098 is an error)
//type:fp
//options:--contracts_p3098:--contracts_p3850:--no_contracts_p3098:--contracts_p3098 --no_contracts;fc
#if TEST_NUMBER <= 2
#if __cpp_contracts_postcondition_captures <= 0
#error __cpp_contracts_postcondition_captures
#endif
#elif defined(__cpp_contracts_postcondition_captures)
#error __cpp_contracts_postcondition_captures defined
#endif
int x;
