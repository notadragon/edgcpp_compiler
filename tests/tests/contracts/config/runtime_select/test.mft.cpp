//remark:contracts: a configuration chooses the run-time semantic of each assertion: postconditions and namespace lib ignored, the rest quick_enforce
//source_files:config.json
//options_all:--contract_evaluation_semantic=quick_enforce --contract_configuration_file=config.json
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rp:-DVIOLATE=2;rp:-DVIOLATE=3;rn:-DVIOLATE=4;rn
// Runs under both back ends: the C-generating one checks quick_enforce and
// ignore itself; through edg-gxx g++ gets the same configuration.  (No
// system headers: the C-generating configuration cannot find them.)
extern "C" int puts(const char *);

int f(int x) pre(x > 0) post(r: r > 1) { return x; }
namespace lib {
  int h(int x) pre(x > 0) { return x; }
}
void g(int x) { contract_assert(x != 4); }

int main() {
  if (VIOLATE == 0) { f(2); lib::h(1); g(0); }
  if (VIOLATE == 1) f(1);              // postcondition: ignored
  if (VIOLATE == 2) lib::h(0);         // namespace lib: ignored
  if (VIOLATE == 3) f(0);              // precondition: traps
  if (VIOLATE == 4) g(4);              // contract_assert: traps
  puts("end");
  return 0;
}
