//remark:contracts: P3097 raises __cpp_contracts above P2900's 202502L
//type:fp
//options:--contracts_p3097:--contracts_p3850:--no_contracts_p3097
#if TEST_NUMBER == 3
#if __cpp_contracts != 202502L
#error __cpp_contracts
#endif
#elif __cpp_contracts <= 202502L
#error __cpp_contracts
#endif
struct B { virtual int f(int x)
#if TEST_NUMBER != 3
  pre(x > 0)
#endif
  ; };
