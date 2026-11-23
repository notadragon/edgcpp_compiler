//remark:contracts: quick_enforce traps on a violation, without calling the handler
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn:-DVIOLATE=3;rn
// Also runs under the C-generating back end, which supports quick_enforce.
// (No system headers: the C-generating configuration cannot find them.)
extern "C" int puts(const char *);

int f(int x) pre(x > 0) post(r: r > 1) { return x; }
void g(int x) { contract_assert(x != 3); }

int main() {
  if (VIOLATE == 0) { f(2); g(0); }
  if (VIOLATE == 1) f(0);
  if (VIOLATE == 2) f(1);
  if (VIOLATE == 3) g(3);
  puts("end");
  return 0;
}
