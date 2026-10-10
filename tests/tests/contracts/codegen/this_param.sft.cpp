//remark:contracts: checks naming "this" in the definition of a member function declared elsewhere, and in a destructor
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn:-DVIOLATE=3;rn:-DVIOLATE=4;rn
// A predicate scanned in a declaration other than the definition has the
// type of "this" in a predicate (pointer to const); its check in the
// definition names the definition's "this" parameter.  Also runs under the
// C-generating back end (no system headers there).
extern "C" int puts(const char *);

struct T {
  int n;
  int get() const pre(n >= 0) post(r: r == this->n);
  void set(int v) pre(this->n != v);
  ~T() pre(n != 3);
};
int T::get() const { return n; }
void T::set(int v) { n = v; }
T::~T() {}

struct W {
  int value;
  ~W() pre(value >= 0) {}
};

int main() {
  if (VIOLATE == 0) { T t{1}; t.set(2); t.get(); W w{0}; }
  if (VIOLATE == 1) { T t{-1}; t.get(); }   // precondition, implicit this
  if (VIOLATE == 2) { T t{2}; t.set(2); }   // precondition, explicit this
  if (VIOLATE == 3) { T t{3}; }             // destructor's precondition
  if (VIOLATE == 4) { W w{-1}; }            // in-class destructor's
  puts("end");
}
