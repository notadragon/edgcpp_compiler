struct N {
  int m = 1;
  int k = [*this]() mutable { return [*this]() mutable { return 2; }(); }();
  int q = [=] { return 3; }();
};
int main() { N n; return n.k + n.q - 5; }
