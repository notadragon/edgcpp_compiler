//remark: imported from clang:capture-throw-skips-later-dtors.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098 --contract_evaluation_semantic=observe
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3098 -fcontract-evaluation-semantic=observe %libcxx_flags -o %t && %t

// Under observe, a capture initializer that throws is reported and the
// postcondition is skipped; the captures constructed before it are destroyed,
// and the one that threw and the ones after it, never constructed, are not.

#include <contracts>

static int ctors, dtors;
struct Good {
  Good() { ++ctors; }
  Good(const Good &) { ++ctors; }
  ~Good() { ++dtors; }
};
struct Bad {
  Bad() { throw 1; }
  Bad(const Bad &) { throw 1; }
  ~Bad() { ++dtors; }
};

static int reports;
void handle_contract_violation(const std::contracts::contract_violation &) {
  ++reports;
}

int f(int x) post [g = Good(), b = Bad(), h = Good()] (false) { return x; }

int main() {
  f(1);
  if (reports != 1 || ctors != 1 || dtors != 1)
    __builtin_abort();
}

// REQUIRES: contracts-libcxx, native
