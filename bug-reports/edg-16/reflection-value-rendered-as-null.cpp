// The C++-generating back end renders every reflection value as a null
// reflection, spelled "(decltype(^^0){})".
using info = decltype(^^int);
template <info R> struct X { static constexpr bool is_int = (R == ^^int); };
constexpr info r = ^^int;
static_assert(X<^^int>::is_int);
int main() { return X<r>::is_int ? 0 : 1; }
