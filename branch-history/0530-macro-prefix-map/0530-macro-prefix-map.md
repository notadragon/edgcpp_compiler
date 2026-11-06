---
id: 0530-macro-prefix-map
subject: 'Prefix maps: --macro_prefix_map, and edg_gxx.sh passes the prefix maps to the front end []'
depends: [0520-gxx-test-config]
regenerates: []
fixes: []
---

## Rationale

GCC's `-fmacro-prefix-map=OLD=NEW` and `-ffile-prefix-map=OLD=NEW` replace
the prefix OLD of the file name that `__FILE__`, `__BASE_FILE__` and
`__FILE_NAME__` yield, so that an object file built in one directory names
the same files as one built in another.  Through edg_gxx.sh the front end
expands those macros (g++ sees only regenerated, preprocessed C++), so
passing the maps to g++ alone remapped the file names g++ records -- debug
information, `std::source_location`, contract violations, sanitizer
reports -- but not `__FILE__`.

The front end gets the same option, `--macro_prefix_map=OLD=NEW`
(cmd_line.c), which may be repeated.  The file name of an expansion outside
a preprocessing directive has the prefix OLD of the first map whose OLD it
begins with replaced with NEW, the last map given being tried first
(`remap_macro_file_name`, macro.c), as in GCC's `remap_filename`.  An
argument without `=` is an error (the last `=` splits it, as in GCC).  A map
given after `--canon_prefix_map` (until `--no_canon_prefix_map`) compares
its OLD and a file name with a directory with `.`, `..` and symbolic links
resolved, as under GCC's `-fcanon-prefix-map`
(`canonical_file_name`, host_envir.c, with `realpath`).

edg_gxx.sh passes `-fmacro-prefix-map=` and `-ffile-prefix-map=` to the
front end as `--macro_prefix_map=` as well as to g++, in g++'s order of
precedence: g++ adds its file maps after its macro maps, so every
`-ffile-prefix-map=` is tried before any `-fmacro-prefix-map=`, whatever
their order; each is canonical when `-fcanon-prefix-map` was in effect for
it.  eccp.sh accepts the new options, and the suite's eccp-gxx passes them
on, and `-f[no-]canon-prefix-map` and `-f[no-]sanitize...` to edg_gxx.sh.

The tests are `tests/tests/contracts/macro_prefix_map/`: the option for both
back ends (`front_end`), g++'s options through edg-gxx compared with g++'s
own results (`gxx_options`), and the sanitizer reports' file names
(`sanitizer_*`, mirrors of GCC's and Clang's); `prefix_map/` (5000-p4299,
6000-p3100-core) now requires a violation's file name to be the remapped
`__FILE__`.

## Compile gap

None.

## Contents

- src/Changes : /^11\/5\/26 \[\]$/, /^Prefix maps: --macro_prefix_map$/, /^The new --macro_prefix_map=OLD=NEW option, which may be repeated, replaces$/, /^the prefix OLD of the file name that __FILE__, __BASE_FILE__ and$/, /^with "\.", "\.\." and symbolic links resolved, as under GCC's$/, /^precedence \(every -ffile-prefix-map= first\), with -fcanon-prefix-map as$/, /^names it records \(macro prefix maps\)\.$/
- src/cmd_line.c : #1, #6, #12
- src/cmd_line.h : #0, #2
- src/error_msg.txt : /(^|;)ec_cl_invalid_macro_prefix_map;/
- src/host_envir.c : #0
- src/host_envir.h : #1
- src/macro.c : #1, #2
- util/eccp.sh : #0, #2, #3, #6, #8
- util/edg_gxx.sh : #0:18, /^canon_prefix_map=0$/
- dev_tools/bin/eccp-gxx : #0:18
- tests/tests/contracts/macro_prefix_map/* : *
- tests/expectations/edg_x86_64/contracts.log : /^macro_prefix_map\//
