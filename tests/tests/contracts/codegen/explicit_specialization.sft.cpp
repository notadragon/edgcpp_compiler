//remark:contracts: checks of the contract assertions of explicit specializations under quick_enforce
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn:-DVIOLATE=3;rn
// An explicit specialization is checked against its own contract assertions,
// not its template's (which every call below violates): of a function
// template (also one declared with them and defined without) and of a
// member of a class template.  One without contract assertions is not
// checked.  Under the C-generating back end the checks are EDG's; the
// C++-generating back end puts out each specialization with its own, for
// g++ to check.  (A specialization of a member template is in
// explicit_specialization_member.)  (No system headers: the C-generating
// configuration cannot find them.)
extern "C" int puts(const char *);

template <class T> int ft(T t) pre(t > 100) { return 0; }
template <> int ft<int>(int a) pre(a > 5) { return 1; }
template <class T> int fd(T t) pre(t > 100);
template <> int fd<int>(int a) pre(a > 5);
template <> int fd<int>(int b) { return 2; }
template <> int ft<long>(long a) { return 3; }
template <class T> struct B {
  int m(T t) pre(t > 100) { return 0; }
};
template <> int B<int>::m(int a) pre(a > 5) { return 4; }

int main() {
  if (VIOLATE == 0) {
    ft(6); fd(6); ft(1L); B<int>().m(6);
  }
  if (VIOLATE == 1) ft(5);
  if (VIOLATE == 2) fd(5);
  if (VIOLATE == 3) B<int>().m(5);
  puts("end");
  return 0;
}
