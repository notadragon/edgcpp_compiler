// EDG-159 case 8(a): a contract_control using-directive in a header unit.
// The label is reached through a type and a function, not a variable: no
// variable declared in a header unit reaches an importer (see the writeup).
namespace g10ns
{
  struct my_label_t
  {
    using assertion_control_object = my_label_t;
  };
  constexpr my_label_t make_label() { return {}; }
  extern const my_label_t my_label;
}
using contract_control namespace g10ns;
