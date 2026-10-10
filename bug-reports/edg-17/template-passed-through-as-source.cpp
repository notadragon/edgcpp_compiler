// The C++-generating back end prints a template from its tokens and leaves
// its instantiation to the downstream compiler, so the template below reaches
// that compiler as written (a C++26 pack-indexing expression included), while
// the non-template function is regenerated from the IL.
template <class... T> auto first(T... x) { return x...[0] + 1 + 2; }
int f(int a) { return a + 1 + 2; }
int main() { return first(1) + f(1) - 8; }
