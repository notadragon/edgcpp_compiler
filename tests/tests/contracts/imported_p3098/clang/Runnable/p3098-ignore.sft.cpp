//remark: imported from clang:Runnable/p3098-ignore.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098 --contract_evaluation_semantic=ignore
//use_system_includes: true
//linker_options: -lcontracts
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3098 \
// RUN:   -fcontract-evaluation-semantic=ignore %libcxx_flags -o %t
// RUN: %t

// Under ignore semantic: no captures constructed or destroyed.

#include <cstdio>

int ctor_count = 0;
int dtor_count = 0;

struct Tracker {
  Tracker(int) { ++ctor_count; }
  Tracker(const Tracker&) { ++ctor_count; }
  ~Tracker() { ++dtor_count; }
};

int f(int x) post [t = Tracker(x)] (true) { return x; }

int main() {
  f(42);
  if (!(ctor_count == 0)) __builtin_abort();
  if (!(dtor_count == 0)) __builtin_abort();
  std::printf("PASS\n");
}
