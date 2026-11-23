//remark:contracts: checks of the contract assertions of explicit specializations declared in a class under quick_enforce
//require:DO_IL_LOWERING 1
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn
// An explicit specialization declared in a class (CWG727) is checked against
// its own contract assertions, whose predicates name members declared after
// them ([class.mem.general]), not the member template's (which every call
// below violates).  (Only under the edg_x86_64 configuration: in GNU mode,
// as in g++, an explicit specialization cannot be declared in a class.  No
// system headers: that configuration cannot find them.)
extern "C" int puts(const char *);

struct S {
  template <class T> int f(T a) pre(a > 100) { return 0; }
  template <> int f<int>(int a) pre(a > lim()) { return a; }
  static int lim() { return 5; }
};

template <class U> struct C {
  template <class T> int f(T a) pre(a > 100) { return 0; }
  template <> int f<int>(int a) pre(a > lim()) { return a; }
  static U lim() { return 5; }
};

int main() {
  if (VIOLATE == 0) { S().f(6); C<int>().f(6); }
  if (VIOLATE == 1) S().f(5);
  if (VIOLATE == 2) C<int>().f(5);
  puts("end");
  return 0;
}
