//remark:contracts: function contract specifiers in invalid positions, one per case
//type:fn
//cases:3
// Mirror of the invalid positions in Clang's clang/test/Parser/cxx-contracts.cpp
// (EDG-99), one per case because EDG's recovery from each skips the next
// declaration; a predicate without parentheses is parse/errors' third case.
#if TEST_NUMBER == 1
void e1(int x) pre(x > 0) const;     // Error, a cv-qualifier after it
#elif TEST_NUMBER == 2
void e2(int x) pre(x > 0) noexcept;  // Error, noexcept after it
#elif TEST_NUMBER == 3
int e4 pre(true);                    // Error, not a function declarator
#endif
