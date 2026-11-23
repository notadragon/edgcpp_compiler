#include <source_location>
void g(std::source_location);
int main() {
  int x = 1;
  x == 1 ? (void)0 : g(std::source_location::current());   // OK
  1 == 1 ? (void)0 : g(std::source_location::current());   // error
}
