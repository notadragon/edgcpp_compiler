//remark:contracts: P3097 in constant evaluation (notadragon_wg21 DECISIONS.md E180 (5)): the checks a virtual call makes of the contract assertions of the function it names never add ignore to their allowed sets, so with ignore configured for the callee side, for caller-side checks and for constant evaluation, B::f's terminating precondition is still checked in a constant-evaluated virtual call, and its violation makes the call not a constant expression; the ordinary precondition is not checked
//type:fn
//options_sep:!
//options_all:--contracts_p4451 --contracts_p3097 --contract_evaluation_semantic=ignore '--contract_configuration=[{"match":{"caller":true},"output":{"semantic":"ignore"}},{"match":{"constexpr":true},"output":{"semantic":"ignore"}}]'
//options:-DVIOLATE=1!-DVIOLATE=2;fp
// No system headers: the declaration the specifier needs is the test's own.
namespace std::contracts::labels {
  inline constexpr struct terminating_t {} terminating{};
}

struct B {
  constexpr virtual int f(int x) const
    pre<terminating>(x != 1)
    pre(x != 2) {
    return x;
  }
};
struct D : B {
  constexpr int f(int x) const override { return x + 10; }
};
constexpr int call(const B &b, int x) { return b.f(x); }
constexpr D d{};
constexpr int v = call(d, VIOLATE);
static_assert(v == VIOLATE + 10);
