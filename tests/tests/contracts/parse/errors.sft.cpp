//remark:contracts: errors in function contract specifiers
//type:fn
//cases:5
#if TEST_NUMBER == 1
void f() post(r: true);         // Error, result name for a void function
#elif TEST_NUMBER == 2
auto g() post(r: true);         // Error, deduced return type, not a definition
#elif TEST_NUMBER == 3
void h(int x) pre x > 0;        // Error, parenthesis required
#elif TEST_NUMBER == 4
void i(int x) {
  contract_assert x;            // Error, parenthesis required
}
#elif TEST_NUMBER == 5
int j(int x) post(r: r > 0) { return r; }  // Error, r not in scope in body
#endif
