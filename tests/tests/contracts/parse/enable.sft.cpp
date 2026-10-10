//remark:contracts: enabled by default in C++26, by --contracts earlier
//options:--c++26:--c++23 --contracts:--c++23:--c++26 --no_contracts
//type:fp
#if TEST_NUMBER <= 2
#if __cpp_contracts != 202502L
#error __cpp_contracts
#endif
void f(int x) {
  contract_assert(x > 0);
}
#else
#ifdef __cpp_contracts
#error __cpp_contracts
#endif
// contract_assert is not a keyword; pre and post never are.
int contract_assert = 1;
#endif
int pre = 2, post = 3;
