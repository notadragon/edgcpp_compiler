//remark:contracts: a coroutine's postconditions cannot use its parameters, unnamed on the definition
//type:fn
//require:BACK_END_IS_CP_GEN_BE 1
//match_regex:line 21: error: an unnamed value parameter used in a postcondition must be declared const
//match_regex:line 23: error: an unnamed parameter of a coroutine cannot be odr-used in a postcondition
// As GCC and Clang diagnose (EDG-107).  A parameter that is not const is
// reported as such at the start of the body, before it is known to be a
// coroutine's (as when it is named).
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
task c1(const int b) post(b > 0);
task c1(int) { co_return; }                         // Error, not const
task c2(const int b) post(b > 0);
task c2(const int) { co_return; }                   // Error
task c3(const int b) post(flag);
task c3(int) { co_return; }                         // OK
