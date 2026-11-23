//remark:contracts: the C-generating back end accepts P3400 labels (they do not change the semantic yet) under quick_enforce
//options_all:--contracts_p3400 --contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn
// Also runs through g++.  (No system headers.)
extern "C" int puts(const char *);
struct Label { using assertion_control_object = Label; };
namespace L { inline constexpr Label lbl{}; }
using contract_control namespace L;

int f(int x) pre<lbl>(x > 0) { return x; }
int g(int x) { contract_assert<(lbl)>(x != 3); return x; }

int main() {
  if (VIOLATE == 0) { f(1); g(1); }
  if (VIOLATE == 1) f(0);
  if (VIOLATE == 2) g(3);
  puts("end");
  return 0;
}
