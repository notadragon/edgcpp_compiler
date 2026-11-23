//remark:contracts: EDG-82 (stock): with the C-generating back end, a lambda capturing *this nested in a lambda capturing *this in a default member initializer stops the front end when local type names are mangled
//require:DO_IL_LOWERING 1
//type:rp
// The recording is the current, wrong behavior (see bug-reports/edg-82/):
// when this test deviates, the bug may be fixed.  Not contracts-specific.
struct N {
  int m = 1;
  int k = [*this]() mutable { return [*this]() mutable { return 2; }(); }();
  int q = [=] { return 3; }();
};
int main() { N n; return n.k + n.q - 5; }
