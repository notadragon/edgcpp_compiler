//remark:contracts: through edg-gxx g++ warns at a declaration with a configured "C++" dynamic selector's name but not its type (-Wcontract-configuration)
//require:BACK_END_IS_CP_GEN_BE 1
//type:cp
//source_files:selectors.json
//options_all:--contract_configuration_file=selectors.json
//options:-DQUIET=0;cp:-DQUIET=1 -Wno-contract-configuration;cp
// GCC's p3595-dynamic-selector-wrong-type.C and its -quiet run (GCC-672,
// mirrored by Clang's CLANG-655; DECISIONS.md E37 (a) in notadragon_wg21).
// The front end does not issue this warning: edg-gxx passes the
// configuration to g++, which warns on the generated C++ at the source
// locations cpfe-cp's #line directives give, so every edg-gxx compilation
// that produces code has it; -fsyntax-only, which stops after cpfe-cp, and
// front-end-only runs have not (DECISIONS.md E68 (c)).  Case 1 records g++'s
// six warnings, at the six declarations marked below; case 2
// (-Wno-contract-configuration) records none.  A function of that name with
// parameters, a template, and a "C"-linkage selector name are not warned
// about.

namespace std::contracts {
  enum class evaluation_semantic : unsigned short {
    ignore = 1, observe, enforce, quick_enforce
  };
}

namespace defined {
  int sel () { return 3; }  // warning (g++)
}

namespace declared {
  long sel ();  // warning (g++)
}

namespace var {
  std::contracts::evaluation_semantic sel;  // warning (g++)
}

namespace right {
  std::contracts::evaluation_semantic sel ();
}

namespace other {
  int sel (int);
  template <class T> int sel ();
}

namespace outer {
  inline namespace v1 {
    unsigned short sel ();  // warning (g++)
  }
}

int global_sel ();  // warning (g++)

extern "C" int c_sel ();

namespace redecl {
  int sel ();
  int sel () { return 1; }  // warning (g++)
}

namespace u1 { void f (int x) pre (x > 0) {} }
namespace u2 { void f (int x) pre (x > 0) {} }
namespace u4 { void f (int x) pre (x > 0) {} }
