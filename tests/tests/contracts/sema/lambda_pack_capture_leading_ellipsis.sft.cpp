//remark:contracts: EDG-118 (stock): a lambda simple-capture with the ellipsis before the name ("...xs") is an error, recovered as "xs..."
//type:fn
// Not contracts-specific.  A simple-capture is "identifier ...opt"
// ([expr.prim.lambda.capture]); only an init-capture puts the ellipsis first.
// Each lambda gets one error, and nothing else is reported: the capture is
// recovered as "xs...".  GCC and Clang reject them too.
template <class... T> int g(T... xs) {
  return [...xs] { return (xs + ... + 0); }();      // Error
}
int a = g(1, 2);
template <class... T> int r(T... xs) {
  return [&...xs] { return (xs + ... + 0); }();     // Error
}
int b = r(1, 2);
template <class... T> int t(T... xs) {
  return [...xs...] { return (xs + ... + 0); }();   // Error, once
}
int c = t(1, 2);
auto h = [](auto... xs) {
  return [...xs] { return sizeof...(xs); }();       // Error
};
template <class... T> int ok(T... xs) {
  return [xs..., ...ys = xs] { return (xs + ... + 0) + (ys + ... + 0); }();
}
int d = ok(1, 2);
