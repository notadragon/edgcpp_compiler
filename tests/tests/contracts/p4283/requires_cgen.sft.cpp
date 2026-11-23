//remark:contracts: under quick_enforce an instance that does not satisfy a contract assertion's requires-clause has no check (P4283), in the C-generating back end's checks and through g++
//options_all:--contracts_p4283 --contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn:-DVIOLATE=3;rn
// Also runs through g++.  (No system headers.)
extern "C" int puts(const char *);
template <class T> struct is_int { static constexpr bool value = false; };
template <> struct is_int<int> { static constexpr bool value = true; };
template <class T> concept Int = is_int<T>::value;

template <class T> T f(T x) pre requires Int<T> (x > 0) { return x; }
template <class T> T g(T x) {
  contract_assert requires (T::value > 0) (x.missing());
  contract_assert requires Int<T> (x != 3);
  return x;
}
template <class T> struct S {
  T m(T x) post requires Int<T> (r: r != 4) { return x; }
};

int main() {
  // Discarded: no check.
  f(-1.0); g(3.0); S<double>().m(4.0);
  if (VIOLATE == 0) { f(1); g(1); S<int>().m(1); }
  if (VIOLATE == 1) f(0);
  if (VIOLATE == 2) g(3);
  if (VIOLATE == 3) S<int>().m(4);
  puts("end");
  return 0;
}
