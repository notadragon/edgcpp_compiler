//remark:contracts: with P3097, virtual functions with postconditions whose bodies never return (an infinite loop, a throw), called directly and through a base, compile and run with both back ends, at -O0 and -O2, the postconditions never evaluated
//type:rp
//options_all:--contracts_p3097 --contract_evaluation_semantic=quick_enforce
//options:-O0:-O2
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/post-noreturn-body-p3097.C and
// post-noreturn-body-p3097-O2.C in the gnu_gcc fork, and
// clang/test/Contracts/post-noreturn-body.cpp in the llvm_llvm-project fork.
// No system headers: the test runs under both configurations.
struct B {
  virtual int loop() post(true) { while (true) {} }
  virtual void loop_void() post(true) { while (true) {} }
  virtual int raise() post(r: r > 0) { throw 10; }
  virtual void raise_void() post(false) { throw 20; }
};

struct D : B {
  int loop() override post(false) { while (true) {} }
  void loop_void() override post(false) { while (true) {} }
  int raise() override post(r: r < 0) { throw 100; }
  void raise_void() override post(false) { throw 200; }
};

// Referenced, so that each is emitted, but never called.
volatile bool never = false;

int main() {
  D d;
  B b;
  B *pd = &d;
  B *pb = &b;
  if (never) {
    pd->loop();
    pd->loop_void();
    d.loop();
    pb->loop();
  }
  int caught = 0;
  try { pd->raise(); } catch (int i) { caught += i; }
  try { pd->raise_void(); } catch (int i) { caught += i; }
  try { pb->raise(); } catch (int i) { caught += i; }
  try { pb->raise_void(); } catch (int i) { caught += i; }
  try { d.raise(); } catch (int i) { caught += i; }
  return caught == 100 + 200 + 10 + 20 + 100 ? 0 : 1;
}
