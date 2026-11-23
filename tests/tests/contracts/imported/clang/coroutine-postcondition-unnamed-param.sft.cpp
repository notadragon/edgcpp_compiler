//remark: imported from clang:coroutine-postcondition-unnamed-param.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 38: (?:catastrophic )?error
//match_regex: ", line 42: (?:catastrophic )?error
// EDG: adapted -- c1 gets one error, the const rule's, at the definition's
// parameter (reported at the start of the body, before it is known to be a
// coroutine's; sema/postcondition_coroutine_unnamed); Clang gives both.
// RUN: %clangxx -std=c++26 -fcontracts %libcxx_flags -Wno-unused-command-line-argument -fsyntax-only -Xclang -verify %s
// REQUIRES: contracts-libcxx

// A coroutine whose definition leaves unnamed a by-value parameter that a
// postcondition on another declaration odr-uses: not const, it gets both of
// coroutine-postcondition-param.cpp's diagnostics (the const rule and the
// coroutine rule); const, the coroutine rule alone.  Mirrored from the EDG
// front end, which missed them (EDG-107 in the edgcpp_compiler fork).
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/coroutine-postcondition-unnamed-param.C
// in the gnu_gcc fork; tests/tests/contracts/sema/postcondition_coroutine_unnamed.sft.cpp
// in the edgcpp_compiler fork.

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

task c1(const int b) post(b > 0); // Clang's const error
task c1(int) { co_return; }       // expected-error {{an unnamed value parameter used in a postcondition must be declared const}}
                                  // EDG reports only the const error here;
                                  // Clang adds the coroutine one.
task c2(const int b) post(b > 0);
task c2(const int) { co_return; } // expected-error {{parameter 'b' is odr-used in a postcondition of a coroutine}} \
                                  // expected-note {{a coroutine copies its parameters}}
task c3(const int b) post(flag);
task c3(int) { co_return; }       // OK
