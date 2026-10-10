//remark:contracts: EDG-118 (stock): a lambda simple-capture with the ellipsis before the name ([...xs]) is accepted
//type:fp
// The recording is the current, wrong behavior (see bug-reports/edg-118/):
// when this test deviates, the bug may be fixed.  Not contracts-specific.
// A simple-capture is "identifier ...opt" ([expr.prim.lambda.capture]);
// only an init-capture puts the ellipsis first.  GCC and Clang reject both
// lambdas.  Found beside the P3098 forms of p3098-unexpanded-pack-capture
// (EDG-115).
template <class... T> int g(T... xs) {
  return [...xs] { return (xs + ... + 0); }();      // Error, not given
}
int a = g(1, 2);
auto h = [](auto... xs) {
  return [...xs] { return sizeof...(xs); }();       // Error, not given
};
