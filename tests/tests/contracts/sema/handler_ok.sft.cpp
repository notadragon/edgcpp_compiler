//remark:contracts: valid declarations named handle_contract_violation
//type:fp
namespace std { namespace contracts { class contract_violation; } }
using cv = std::contracts::contract_violation;
void handle_contract_violation(const cv &);            // OK
void handle_contract_violation(const cv &) { }         // OK
namespace N { int handle_contract_violation(int); }    // OK, not global
struct S { int handle_contract_violation(); };         // OK, member
