//remark:contracts: constification selects const overloads and leaves decltype of a name alone
//type:fp
//options_all:--contract_evaluation_semantic=quick_enforce
template <class T, class U> constexpr bool same = false;
template <class T> constexpr bool same<T, T> = true;
struct S {
  constexpr int get() { return 1; }
  constexpr int get() const { return 2; }
  constexpr int g() pre(get() == 2) { return 0; }
  constexpr int h() { contract_assert(get() == 2); return 0; }
  constexpr int k() pre(this->get() == 2) { return 0; }
};
static_assert(S{}.g() == 0);
static_assert(S{}.h() == 0);
static_assert(S{}.k() == 0);
constexpr int d(int i)
  pre(same<decltype(i), int> && same<decltype((i)), const int &>) { return 0; }
static_assert(d(1) == 0);
constexpr int d2(int i) {
  contract_assert(same<decltype(i), int> && same<decltype((i)), const int &>);
  return 0;
}
static_assert(d2(1) == 0);
constexpr int d3(const int i) pre(same<decltype(i), const int>) { return 0; }
static_assert(d3(1) == 0);
constexpr int rn(const int x)
  post(r: same<decltype(r), int> && same<decltype((r)), const int &>) {
  return x;
}
static_assert(rn(1) == 1);
