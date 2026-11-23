//remark: imported from gcc:await-in-predicate.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=enforce
//match_regex: ", line 41: (?:catastrophic )?error
//match_regex: ", line 42: (?:catastrophic )?error
//match_regex: ", line 43: (?:catastrophic )?error
//match_regex: ", line 44: (?:catastrophic )?error
//match_regex: ", line 45: (?:catastrophic )?error
//match_regex: ", line 46: (?:catastrophic )?error
// An await-expression or yield-expression may appear in a contract
// predicate only within a lambda in it ([expr.await]/2, [expr.yield]),
// also within a statement expression in it, and in a coroutine's
// precondition, which used to be accepted and then ICE (GCC-643).  A lambda
// in the predicate containing co_await is valid.
//
// Mirror: clang/test/Contracts/predicate-await-forms.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcoroutines -fcontract-evaluation-semantic=enforce" }

#include <coroutine>

struct A {
  bool await_ready () { return true; }
  void await_suspend (std::coroutine_handle<>) {}
  bool await_resume () { return true; }
};

struct task {
  struct promise_type {
    task get_return_object () { return {}; }
    std::suspend_never initial_suspend () { return {}; }
    std::suspend_never final_suspend () noexcept { return {}; }
    A yield_value (int) { return {}; }
    void return_void () {}
    void unhandled_exception () {}
  };
};

task c1 () { contract_assert (co_await A{}); co_return; }  // { dg-error "contract" }
task c2 () { contract_assert ((co_yield 1)); co_return; }  // { dg-error "contract" }
task c3 () { contract_assert (__extension__ ({ co_await A{}; true; })); co_return; }  // { dg-error "contract" }
task c4 () { contract_assert ((co_await A{}, true)); co_return; }  // { dg-error "contract" }
task c5 (int) pre ((co_await A{}, true)) { co_return; }  // { dg-error "contract" }
task c6 (int) post (((co_yield 1), true)) { co_return; }  // { dg-error "contract" }

task ok ()
{
  contract_assert (([] () -> task { co_await A{}; co_return; } (), true));  // OK
  co_return;
}
