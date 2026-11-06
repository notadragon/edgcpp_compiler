//remark:contracts: a coroutine's postconditions cannot use its parameters
//type:fn
//require:BACK_END_IS_CP_GEN_BE 1
#include <coroutine>
struct task {
  struct promise_type {
    task get_return_object() { return {}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
};
bool flag = true;
task co(const int x) post(x > 0) { co_return; }     // Error
task co2(const int &x) post(x > 0) { co_return; }   // OK, a reference
task co3(const int x) post(flag) { co_return; }     // OK
