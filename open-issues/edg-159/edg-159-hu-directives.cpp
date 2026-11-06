// Importer for edg-159-hu-directives.h.  Expected: one error, at g4.
import "edg-159-hu-directives.h";

// The global directive: unqualified lookup.
int g1 () { return hu1_v(); }

// The nested directive: qualified lookup into hu1_a ...
int g2 () { return hu1_a::hu1_w(); }

// ... and unqualified lookup from within hu1_a.
namespace hu1_a
{
  int g3 () { return hu1_w(); }
}

// Not an ordinary global name: the nested directive is in hu1_a only.
int g4 () { return hu1_w(); }  // Error
