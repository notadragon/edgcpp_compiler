//remark: imported from gcc:p3097-pure-virtual.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3097 --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// P3097: Contracts on pure virtual functions.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p3097 -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

static int violation_count = 0;

void handle_contract_violation(const std::contracts::contract_violation&) {
  ++violation_count;
}

struct Abstract {
  virtual void f(int x) pre(x > 0) = 0;
};

struct Concrete : Abstract {
  void f(int x) override pre(x > -100) { }
};

int main() {
  Concrete c;
  Abstract& a = c;

  violation_count = 0;
  a.f(-1);  // Abstract pre fails (x>0), Concrete pre passes (x>-100)
  if (violation_count != 1) __builtin_abort();

  violation_count = 0;
  a.f(5);  // both pass
  if (violation_count != 0) __builtin_abort();
}
