//remark:contracts: a postcondition on a function whose body never returns (an infinite loop, a throw, a [[noreturn]] call), with and without a result name, returning int, a class or void, in a template and a lambda, is never evaluated, and the program compiles and runs with both back ends, at -O0 and -O2
//type:rp
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-O0:-O2
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/post-noreturn-body.C and
// post-noreturn-body-O2.C in the gnu_gcc fork, and
// clang/test/Contracts/post-noreturn-body.cpp in the llvm_llvm-project fork.
// No system headers: the test runs under both configurations.
[[noreturn]] void die() { throw 7; }

int loop_int() post(true) { while (true) {} }
int loop_int_named() post(r: r > 0) { while (true) {} }
void loop_void() post(true) { while (true) {} }
int loop_int_for() post(false) { for (;;) {} }

int throw_int() post(true) { throw 1; }
int throw_int_named() post(r: r > 0) { throw 2; }
void throw_void() post(false) { throw 3; }
int throw_int_false() post(false) { throw 4; }
int die_int() post(r: r > 0) { die(); }
void die_void() post(false) { die(); }
struct Small { int a; };
struct Large { int a, b, c, d, e; };
Small die_small() post(r: r.a > 0) { die(); }
Large die_large() post(r: r.a > 0) { die(); }

// A precondition and a contract_assert in such a body are checked as usual,
// and a body that returns on one path checks the postcondition there.
int pre_then_throw(int x) pre(x > 0) post(false) {
  contract_assert(x > 0);
  throw x;
}
int returns_sometimes(int x) post(r: r == 8) {
  if (x > 0) throw x;
  return 8;
}

template <typename T>
T loop_tmpl() post(true) { while (true) {} }
template <typename T>
T throw_tmpl() post(false) { throw 5; }

auto loop_lambda = []() -> int post(true) { while (true) {} };
auto throw_lambda = []() -> int post(false) { throw 6; };

// Referenced, so that each is emitted, but never called.
volatile bool never = false;

int main() {
  if (never) {
    loop_int();
    loop_int_named();
    loop_void();
    loop_int_for();
    loop_tmpl<int>();
    loop_lambda();
  }
  int caught = 0;
  try { throw_int(); } catch (int i) { caught += i; }
  try { throw_int_named(); } catch (int i) { caught += i; }
  try { throw_void(); } catch (int i) { caught += i; }
  try { throw_int_false(); } catch (int i) { caught += i; }
  try { throw_tmpl<int>(); } catch (int i) { caught += i; }
  try { throw_lambda(); } catch (int i) { caught += i; }
  try { die_int(); } catch (int i) { caught += i; }
  try { die_void(); } catch (int i) { caught += i; }
  try { die_small(); } catch (int i) { caught += i; }
  try { die_large(); } catch (int i) { caught += i; }
  try { pre_then_throw(9); } catch (int i) { caught += i; }
  try { returns_sometimes(10); } catch (int i) { caught += i; }
  if (caught != 1 + 2 + 3 + 4 + 5 + 6 + 4 * 7 + 9 + 10) return 1;
  return returns_sometimes(0) == 8 ? 0 : 2;
}
