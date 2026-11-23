//remark:contracts: P3098 through edg-gxx: g++ checks the captures as written, on members, templates and classes passed by value
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contracts_p3098 --contract_evaluation_semantic=observe
//match_regex:^violations 3$
#include <contracts>
#include <cstdio>
#include <string>

static int n = 0;
void handle_contract_violation(const std::contracts::contract_violation &) {
  ++n;
}

struct vec {
  int size_ = 0;
  int size() const { return size_; }
  void push() post [old = size()] (size() == old + 1) { size_ += 1; }
  void push_bad() post [old = size()] (size() == old + 1) { size_ += 2; }
};
template<class T> T id(T x) post [x] (r: r == x) { return x; }
std::string upper(std::string s) post [s] (r: r.size() == s.size()) {
  return s + "!";
}

int main() {
  vec v;
  v.push();                 // OK
  v.push_bad();             // violation
  id(3);                    // OK
  id(std::string("a"));     // OK
  upper("abc");             // violation
  [](int x) -> int post [y = x] (r: r == y) { return x + 1; }(1);  // violation
  std::printf("violations %d\n", n);
  return 0;
}
