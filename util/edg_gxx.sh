#!/bin/bash

# Run the EDG C++-generating front end (cpfe-cp) into a GNU g++ to compile
# C++.  Interface and command-line options are those of g++.
#
# Unlike eccp, which lowers to C and so implements exception handling with
# setjmp/longjmp, this regenerates C++ and lets g++ compile it.  Exceptions,
# RTTI and the rest of the Itanium ABI are then g++'s own, so code produced
# here links against, throws through and catches from that g++'s libstdc++
# (and any other library it built) exactly as g++-compiled code does.
#
# Layout.  This script is installed as <prefix>/bin/edg-gxx, beside
# <prefix>/bin/cpfe-cp, and <prefix>/base is the EDG_BASE directory (it holds
# lib/predefined_macros.txt).  The g++ to pair with is $EDG_GXX, else the one
# named in <prefix>/base/edg_gxx_config, else "g++" from PATH.  Its include
# directories and version are read from it on every run, so the same install
# works against any g++ without being rebuilt.
#
# Options.  Every option not listed below goes to g++ only.  Those that
# affect how the source is interpreted go to cpfe-cp only, since g++ sees
# only already-preprocessed, already-checked output:
#   -D, -U, -I, -isystem, -include, -std=, -f[no-]exceptions, -f[no-]rtti
# -w goes to both.  -E and -fsyntax-only stop after cpfe-cp.  Raw front end
# options are passed with -Xedg <option> or --edg=<option>.
#
# Contracts.  The contract options -f[no-]contracts and
# -fcontract-evaluation-semantic= go to both.  cpfe-cp checks the contract
# assertions as the front end does and puts them out as written, and g++
# implements them: it generates the checks with the given semantic, links the
# contracts runtime, and makes a user-defined handle_contract_violation the
# violation handler.  The comment of a violation is g++'s printing of the
# predicate (-fcontract-comment-from-expression, when g++ has it).
# The configuration options of P3595, -fcontract-configuration=,
# -fcontract-configuration-file= and -fcontract-group-evaluation-semantic=,
# go to both: g++ chooses the semantics of the checks with them, and the front
# end the semantics of constant evaluation.  -Wno-contract-configuration
# also silences the front end's warnings about the configuration.
# -f[no-]contracts-p3850 (every extension paper) goes to both; g++ gets it
# unchanged, so it enables there even the papers the front end does not
# implement yet.
# -f[no-]contracts-p3097 goes to both.
#
# Environment:
#   EDG_GXX           the g++ to pair with
#   EDG_GXX_VERBOSE   when nonzero, print each command before running it
#                     (also enabled by -v)
#   EDG_GXX_KEEP      when nonzero, keep the generated C++ and print where

set -o pipefail

driver_name=edg-gxx

script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
prefix=$(dirname "$script_dir")

CPFE_CP=${CPFE_CP-$script_dir/cpfe-cp}
export EDG_BASE=${EDG_BASE-$prefix/base}

if [ -z "${EDG_GXX:-}" ] && [ -f "$EDG_BASE/edg_gxx_config" ] ; then
  . "$EDG_BASE/edg_gxx_config"
fi
GXX=${EDG_GXX:-g++}

verbose=${EDG_GXX_VERBOSE:-0}
keep=${EDG_GXX_KEEP:-0}

die() {
  echo "$driver_name: $*" >&2
  exit 1
}

run() {
  if [ "$verbose" != 0 ] ; then
    printf '%q ' "$@" >&2
    echo >&2
  fi
  # The shell's own report of a command killed by a signal ("line N: <pid>
  # Aborted") names a process id, so it is discarded (only the command's
  # stderr reaches ours) and replaced by a message that does not vary.
  local status=0
  { "$@" 2>&3 3>&-; } 3>&2 2>/dev/null || status=$?
  if [ $status -gt 128 ] ; then
    echo "$driver_name: $(basename "$1") terminated by signal $((status - 128))" >&2
  fi
  return $status
}

[ -x "$CPFE_CP" ] || die "front end not found: $CPFE_CP"
command -v "$GXX" > /dev/null 2>&1 || die "g++ not found: $GXX"

#
# Classify the command line.
#
edg_args=()
gxx_args=()
sources=()
stop_after_edg=0
edg_only_mode=""
explicit_lang=""

while [ $# -gt 0 ] ; do
  arg=$1
  shift
  case $arg in
    -D|-U)
      [ $# -gt 0 ] || die "missing argument to $arg"
      edg_args+=("$arg$1"); shift ;;
    -D*|-U*)
      edg_args+=("$arg") ;;
    -I)
      [ $# -gt 0 ] || die "missing argument to $arg"
      edg_args+=("--include_directory=$1"); shift ;;
    -I*)
      edg_args+=("--include_directory=${arg#-I}") ;;
    -isystem)
      [ $# -gt 0 ] || die "missing argument to $arg"
      edg_args+=("--sys_include=$1"); shift ;;
    -include)
      [ $# -gt 0 ] || die "missing argument to $arg"
      edg_args+=("--preinclude=$1"); shift ;;
    -std=c++98|-std=gnu++98|-std=c++03|-std=gnu++03)
      edg_args+=("--c++03"); gxx_args+=("$arg") ;;
    -std=c++0x|-std=gnu++0x|-std=c++11|-std=gnu++11)
      edg_args+=("--c++11"); gxx_args+=("$arg") ;;
    -std=c++1y|-std=gnu++1y|-std=c++14|-std=gnu++14)
      edg_args+=("--c++14"); gxx_args+=("$arg") ;;
    -std=c++1z|-std=gnu++1z|-std=c++17|-std=gnu++17)
      edg_args+=("--c++17"); gxx_args+=("$arg") ;;
    -std=c++2a|-std=gnu++2a|-std=c++20|-std=gnu++20)
      edg_args+=("--c++20"); gxx_args+=("$arg") ;;
    -std=c++2b|-std=gnu++2b|-std=c++23|-std=gnu++23)
      edg_args+=("--c++23"); gxx_args+=("$arg") ;;
    -std=c++2c|-std=gnu++2c|-std=c++26|-std=gnu++26)
      edg_args+=("--c++26"); gxx_args+=("$arg") ;;
    -std=*)
      die "unsupported language standard: $arg" ;;
    -fexceptions)
      edg_args+=("--exceptions"); gxx_args+=("$arg") ;;
    -fno-exceptions)
      edg_args+=("--no_exceptions"); gxx_args+=("$arg") ;;
    -frtti)
      edg_args+=("--rtti"); gxx_args+=("$arg") ;;
    -fno-rtti)
      edg_args+=("--no_rtti"); gxx_args+=("$arg") ;;
    -w)
      edg_args+=("--no_warnings"); gxx_args+=("$arg") ;;
    -fcontracts)
      edg_args+=("--contracts"); gxx_args+=("$arg") ;;
    -fno-contracts)
      edg_args+=("--no_contracts"); gxx_args+=("$arg") ;;
    -fcontract-evaluation-semantic=*)
      edg_args+=("--contract_evaluation_semantic=${arg#*=}")
      gxx_args+=("$arg") ;;
    -fcontract-configuration=*)
      edg_args+=("--contract_configuration=${arg#*=}")
      gxx_args+=("$arg") ;;
    -fcontract-configuration-file=*)
      edg_args+=("--contract_configuration_file=${arg#*=}")
      gxx_args+=("$arg") ;;
    -fcontract-group-evaluation-semantic=*)
      edg_args+=("--contract_group_evaluation_semantic=${arg#*=}")
      gxx_args+=("$arg") ;;
    -Wno-contract-configuration)
      edg_args+=("--diag_suppress=contract_configuration")
      gxx_args+=("$arg") ;;
    -fcontracts-p3850)
      edg_args+=("--contracts_p3850"); gxx_args+=("$arg") ;;
    -fno-contracts-p3850)
      edg_args+=("--no_contracts_p3850"); gxx_args+=("$arg") ;;
    -fcontracts-p3097)
      edg_args+=("--contracts_p3097"); gxx_args+=("$arg") ;;
    -fno-contracts-p3097)
      edg_args+=("--no_contracts_p3097"); gxx_args+=("$arg") ;;
    -E)
      edg_only_mode=preprocess; stop_after_edg=1 ;;
    -fsyntax-only)
      edg_only_mode=syntax; stop_after_edg=1 ;;
    -Xedg)
      [ $# -gt 0 ] || die "missing argument to $arg"
      edg_args+=("$1"); shift ;;
    --edg=*)
      edg_args+=("${arg#--edg=}") ;;
    -v)
      verbose=1; gxx_args+=("$arg") ;;
    -x)
      [ $# -gt 0 ] || die "missing argument to $arg"
      explicit_lang=$1; shift ;;
    # Options whose argument is a separate word, so that the argument is
    # not mistaken for a source file.
    -o|-L|-l|-Xlinker|-Xassembler|-Xpreprocessor|-MF|-MT|-MQ|-B|-T|-u|-z)
      [ $# -gt 0 ] || die "missing argument to $arg"
      gxx_args+=("$arg" "$1"); shift ;;
    -*)
      gxx_args+=("$arg") ;;
    *)
      case ${explicit_lang:-${arg##*.}} in
        c++|cpp|cc|cxx|c++m|cppm|ixx|C|CC|CPP)
          sources+=("$arg") ;;
        *)
          # Objects, archives, and anything else: g++ decides.
          gxx_args+=("$arg") ;;
      esac ;;
  esac
done

#
# Read the paired g++'s configuration: its version, in the form EDG's
# --gnu_version expects (major*10000 + minor*100 + patch), and its #include
# <...> search list, which becomes cpfe-cp's system include path.
#
gxx_version=$("$GXX" -dumpfullversion 2> /dev/null) \
  || die "cannot determine the version of $GXX"
IFS=. read -r v_major v_minor v_patch <<< "$gxx_version"
gnu_version=$(( v_major * 10000 + ${v_minor:-0} * 100 + ${v_patch:-0} ))

sys_includes=()
in_list=0
while IFS= read -r line ; do
  case $line in
    "#include <...> search starts here:") in_list=1 ;;
    "End of search list.") in_list=0 ;;
    *)
      if [ $in_list = 1 ] ; then
        sys_includes+=("--sys_include=${line# }")
      fi ;;
  esac
done < <("$GXX" -x c++ -E -v /dev/null 2>&1 > /dev/null)
[ ${#sys_includes[@]} -gt 0 ] \
  || die "cannot determine the include search list of $GXX"

edg_common=(
  --gnu_version="$gnu_version"
  --exceptions
  --no_wrap_diagnostics
  --diag_suppress last_line_incomplete
  -D__CHAR_BIT__=8
  "${sys_includes[@]}"
)

if [ ${#sources[@]} -eq 0 ] ; then
  [ $stop_after_edg = 0 ] || die "no input files"
  # Link-only (or query-only) invocation: nothing for the front end to do.
  exec "$GXX" "${gxx_args[@]}"
fi

if [ $stop_after_edg = 1 ] ; then
  # The exit status is the front end's (the last nonzero one), so that a
  # catastrophic error stays distinguishable from an ordinary one.
  status=0
  for src in "${sources[@]}" ; do
    if [ "$edg_only_mode" = preprocess ] ; then
      run "$CPFE_CP" "${edg_common[@]}" "${edg_args[@]}" -E "$src"
    else
      run "$CPFE_CP" "${edg_common[@]}" "${edg_args[@]}" --no_code_gen \
          "$src"
    fi
    rc=$?
    [ $rc = 0 ] || status=$rc
  done
  exit $status
fi

# The generated C++ names the original sources in its #line directives, but
# its columns are not theirs, so g++ must not read a contract violation's
# comment back from those files.  -fcontract-comment-from-expression (our
# GCC's) makes it print the predicate instead; pass it when g++ takes it.
"$GXX" -fcontract-comment-from-expression -x c++ -E /dev/null \
    > /dev/null 2>&1 && gxx_args+=(-fcontract-comment-from-expression)

tmp_dir=$(mktemp -d "${TMPDIR:-/tmp}/edg-gxx.XXXXXXXX") \
  || die "cannot create a temporary directory"
if [ "$keep" = 0 ] ; then
  trap 'rm -rf "$tmp_dir"' EXIT
fi

#
# Generate C++ for each source.  Each generated file keeps its source's base
# name, in a directory of its own, so that g++ derives the same default
# output names (foo.o for foo.cpp under -c) it would for the original.
#
generated=()
index=0
for src in "${sources[@]}" ; do
  index=$((index + 1))
  base=$(basename "$src")
  mkdir -p "$tmp_dir/$index"
  out="$tmp_dir/$index/${base%.*}.cpp"
  run "$CPFE_CP" "${edg_common[@]}" "${edg_args[@]}" \
      --gen_c_file_name="$out" "$src" || exit $?
  generated+=("$out")
done

if [ "$keep" != 0 ] ; then
  echo "$driver_name: generated C++ kept in $tmp_dir" >&2
fi

run "$GXX" "${gxx_args[@]}" -x c++ "${generated[@]}" -x none
