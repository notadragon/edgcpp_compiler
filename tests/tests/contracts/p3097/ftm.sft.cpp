//remark:contracts: P3097 raises __cpp_contracts to the fixed 202612L, above P2900's 202502L and P4451's 202611L (P4451, which C++26 enables, on or off); without P3097, C++26 gives P4451's 202611L
//type:fp
//options:--contracts_p3097 --no_contracts_p4451:--contracts_p3850:--no_contracts_p3097 --no_contracts_p4451:--contracts_p3097:--c++26
#if TEST_NUMBER == 3
#if __cpp_contracts != 202502L
#error __cpp_contracts
#endif
#elif TEST_NUMBER == 5
#if __cpp_contracts != 202611L
#error __cpp_contracts
#endif
#elif __cpp_contracts != 202612L
#error __cpp_contracts
#endif
struct B { virtual int f(int x)
#if TEST_NUMBER < 3 || TEST_NUMBER == 4
  pre(x > 0)
#endif
  ; };
