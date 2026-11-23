//remark: imported from gcc:p3400-callerside-always-enforce.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3400-callerside-always-enforce.json
//options: --c++26 --contracts_p3400 --contract_evaluation_semantic=ignore --contract_configuration_file=p3400-callerside-always-enforce.json
//use_system_includes: true
//linker_options: -lcontracts
// EDG: g++ makes the caller-side checks (through edg-gxx); the
// C-generating back end makes none, so this runs under edg_x86_64_gxx
// only.
// EDG: adapted -- the original is copied beside this file and included,
// so the configuration's locations (its file name and line numbers)
// name it as GCC's do.
// p3400-callerside-always-enforce.C under the options of the original.

#include "p3400-callerside-always-enforce.C"
