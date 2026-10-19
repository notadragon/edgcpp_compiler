//remark:contracts: function contract specifiers and contract_assert parse
//type:fp
int f(const int x) pre(x > 0) post(r: r > x) { return x + 1; }
int g(const int x) pre(x != 0) pre(x != 1) post(x >= 0);
int g(const int x) { return x; }

// The result name can be any identifier, including pre or post.
int h(const int a, const int b) post(pre: pre >= a) post(post: post >= b) {
  return a > b ? a : b;
}

// A result name is visible only in its own predicate.
int k(const int x) post(r: r == x) post(x > 0) { return x; }

// Lambdas, with and without a parameter list.
auto l1 = [](const int y) pre(y > 0) post(y > 0) { return y; };
auto l2 = [] pre(true) { return 1; };
auto l3 = [](const int y) -> int post(r: r == y) { return y; };

constexpr int c(int x) pre(x > 0) {
  contract_assert(x > 0);
  return x;
}

void s(int z) {
  contract_assert(z > 0);
  contract_assert(f(z) > 1 && g(z) != 0);
  if (z) contract_assert(z != 0);
}

// pre and post remain ordinary identifiers elsewhere.
int pre = 1;
int post(int pre) { return pre; }
