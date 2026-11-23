struct N {
  int m = 1;
  int g = [*this]() mutable { return [this] { return ++m; }(); }();
};
int main() { N n; return n.g - 2; }
