//remark:contracts: an await-expression or yield-expression in a contract predicate only in a lambda-expression
//type:fn
//match_regex:line 23: error: "co_await" cannot appear in a contract predicate other than in a lambda-expression
//match_regex:line 24: error: "co_yield" cannot appear in a contract predicate other than in a lambda-expression
//match_regex:line 25: error: "co_await" cannot appear in a contract predicate other than in a lambda-expression
//require:BACK_END_IS_CP_GEN_BE 1
#include <coroutine>
struct A {
  bool await_ready() { return true; }
  void await_suspend(std::coroutine_handle<>) {}
  bool await_resume() { return true; }
};
struct task {
  struct promise_type {
    task get_return_object() { return {}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    A yield_value(int) { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
};
task c1() { contract_assert(co_await A{}); co_return; }                // Error
task c2() { contract_assert((co_yield 1)); co_return; }                // Error
task c3() { contract_assert(({ co_await A{}; })); co_return; }         // Error
task c4() {
  contract_assert(([]() -> task { co_await A{}; co_return; }(), true));  // OK
  co_return;
}
