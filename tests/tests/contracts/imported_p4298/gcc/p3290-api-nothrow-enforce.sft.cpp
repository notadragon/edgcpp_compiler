//remark: imported from gcc:p3290-api-nothrow-enforce.C
//type: ra
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3290
//use_system_includes: true
//linker_options: -lcontracts
//match_regex: contract handler ran
// P3290: nothrow enforce overload invokes handler with correct fields.
// This TU does NOT enable -fcontracts-p4298, so the nothrow_t overload binds
// to the plain std::contracts variant and reports classic enforce.  (P4298's
// noexcept_enforce is reported only when the caller sets -fcontracts-p4298;
// see p4298-p3290-nothrow.C for that case.)
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p3290" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
// { dg-shouldfail "contract violation" }

#include <contracts>
#include <cstdio>
#include <cstdlib>
#include <new>
#include <cstring>

void handle_contract_violation(const std::contracts::contract_violation& v) {
  std::fputs ("contract handler ran\n", stderr);
  if (v.kind() != std::contracts::assertion_kind::manual)
    std::_Exit (0);
  if (v.semantic() != std::contracts::evaluation_semantic::enforce)
    std::_Exit (0);
  if (v.detection_mode() != std::contracts::detection_mode::unspecified)
    std::_Exit (0);
  if (!v.comment() || std::strcmp(v.comment(), "nothrow enforce") != 0)
    std::_Exit (0);
}

int main() {
  std::contracts::handle_enforced_contract_violation(
      std::nothrow, "nothrow enforce");
  std::_Exit (0);
}

// A failed check exits normally, which dg-shouldfail reports as a failure;
// the handler must have run.
// { dg-output "contract handler ran" }
