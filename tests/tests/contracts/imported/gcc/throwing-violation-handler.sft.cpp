//remark: imported from gcc:throwing-violation-handler.cc
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
#include <contracts>

void handle_contract_violation(const std::contracts::contract_violation&)
{
  throw 666;
}

/* ODR hack to the hilt - f is actually noexcept, and has a contract.
   We do this to avoid the call site knowing that, so that we verify
   that the function terminates when it calls the throwing handler
   that we use above. */
int f(int);

extern bool f_result;

void g()
{
  try {
    f(-42);
  } catch (...) {
    f_result = false;
  }
}
