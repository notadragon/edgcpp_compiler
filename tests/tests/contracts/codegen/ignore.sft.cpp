//remark:contracts: ignore does not evaluate predicates
//type:rp
//options_all:--contract_evaluation_semantic=ignore
// Also runs under the C-generating back end, which supports ignore.  (No
// system headers: the C-generating configuration cannot find them.)
extern "C" int puts(const char *);

int evaluations = 0;
bool count(bool b) { ++evaluations; return b; }

int f(int x) pre(count(x > 0)) post(r: count(r > 100)) { return x; }
void g() { contract_assert(count(false)); }

int main() {
  f(0);
  g();
  puts(evaluations == 0 ? "no evaluations" : "evaluated");
  return evaluations;
}
