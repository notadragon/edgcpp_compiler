// EDG-159 cases 1 and 2: a using-directive at global scope and one nested
// in a namespace, in a header unit.  Functions, not variables: no variable
// declared in a header unit reaches an importer (see the writeup).
namespace hu1_n
{
  inline int hu1_v() { return 1; }
}
using namespace hu1_n;

namespace hu1_a
{
  namespace hu1_m
  {
    inline int hu1_w() { return 2; }
  }
  using namespace hu1_m;
}
