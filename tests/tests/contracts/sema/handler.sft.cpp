//remark:contracts: invalid replacements of the contract-violation handler
//type:fn
//cases:6
namespace std { namespace contracts { class contract_violation; } }
using cv = std::contracts::contract_violation;
#if TEST_NUMBER == 1
inline void handle_contract_violation(const cv &) { }  // Error, inline
#elif TEST_NUMBER == 2
extern "C" void handle_contract_violation(const cv &); // Error, C linkage
#elif TEST_NUMBER == 3
int handle_contract_violation(const cv &);             // Error, return type
#elif TEST_NUMBER == 4
void handle_contract_violation(cv &);                  // Error, not const
#elif TEST_NUMBER == 5
void handle_contract_violation(const cv &, int);       // Error, parameters
#elif TEST_NUMBER == 6
void handle_contract_violation(const cv &) = delete;   // Error, deleted
#endif
