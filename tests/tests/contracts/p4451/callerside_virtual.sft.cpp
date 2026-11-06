//remark:contracts: P3097 (notadragon_wg21 DECISIONS.md E180 (5)): the checks a virtual call makes of the contract assertions of the function it names never add ignore to their allowed sets -- the front end's interface wrapper for the C-generating back end, and g++'s through edg-gxx -- so with ignore configured for the callee side and for caller-side checks, B::f's terminating precondition #1 and postcondition #3 are still checked around D::f (quick_enforce with the C-generating back end, enforce through g++) and only the ordinary #2 is skipped
//options_sep:!
//options_all:--contracts_p4451 --contracts_p3097 --contract_evaluation_semantic=ignore '--contract_configuration=[{"match":{"caller":true},"output":{"semantic":"ignore"}}]'
//options:-DVIOLATE=0;rp!-DVIOLATE=1;rn!-DVIOLATE=2;rp!-DVIOLATE=3;rn
// No system headers (also runs under the C-generating back end): the
// declaration the specifier needs is the test's own.  Each check prints its
// number first, so the output shows which checks were made; a failed
// terminating check ends the program.
extern "C" int puts(const char *);
extern "C" int fflush(void *);
namespace std::contracts::labels {
  inline constexpr struct terminating_t {} terminating{};
}

bool check(int id) {
  const char text[2] = { char('0' + id), '\0' };
  puts(text);
  fflush(0);
  return id != VIOLATE;
}

struct B {
  virtual void f()
    pre<terminating>(check(1))
    pre(check(2))
    post<terminating>(check(3));
};

void B::f() {}

struct D : B {
  void f() override { puts("D::f"); fflush(0); }
};

void call(B &b) { b.f(); }

int main() {
  D d;
  call(d);
  puts("end");
  return 0;
}
