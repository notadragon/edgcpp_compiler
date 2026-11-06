//remark: imported from gcc:p4451-callerside-seq.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4451-callerside-seq.json
//options: --c++26 --contracts_p4451 --contract_configuration_file=p4451-callerside-seq.json
//use_system_includes: true
//linker_options: -lcontracts
// EDG: g++ makes the caller-side checks (through edg-gxx); the
// C-generating back end makes none, so this runs under edg_x86_64_gxx
// only.
// EDG: adapted -- the original is copied beside this file and included,
// so the configuration's locations (its file name and line numbers)
// name it as GCC's do.
// p4451-callerside-seq.C under the options of the original.

#include "p4451-callerside-seq.C"
