extern "C" int puts(const char *);
struct M { ~M() { puts("member destroyed"); } };
struct S { int v; M m; };
int main() { if (puts("") > 0) { S s(0); } }
