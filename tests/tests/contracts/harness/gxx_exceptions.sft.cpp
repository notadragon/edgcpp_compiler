//remark:gxx configuration: catch an exception thrown by libstdc++
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
// Under eccp the C-generating back end cannot catch an exception thrown by
// g++-compiled code; this passes only when the test runs through edg-gxx.
#include <cstdio>
#include <stdexcept>
#include <vector>

int main() {
  std::vector<int> v(1);
  try {
    (void)v.at(5);
  } catch (const std::out_of_range&) {
    std::puts("caught");
    return 0;
  }
  return 1;
}
