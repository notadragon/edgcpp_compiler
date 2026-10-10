/*
Part of the EDG Compiler Project, under the Apache License v2.0 with LLVM
Exceptions.
See https://edgcpp.org/LICENSE.txt for license information.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
*/

/*

contract_config.c -- The configuration of contract evaluation semantics
                     (P3595)

The evaluation semantic of each contract assertion is chosen by an ordered
list of configuration entries, the first that matches the assertion giving
it.  The entries come from the --contract_configuration (JSON text),
--contract_configuration_file (a JSON file) and
--contract_group_evaluation_semantic options, in command-line order, followed
by a catch-all for the --contract_evaluation_semantic option.  The JSON form
is our GCC's (-fcontract-configuration, contracts-config.cc): an array of
objects, each with an optional "match" object and an "output" object.

With the C++-generating back end the contract assertions are put out as
written, and g++, given the same options, chooses their semantics at run
time; the front end chooses semantics only where it evaluates assertions
itself: in constant evaluation, and for the checks it generates for the
C-generating back end (see CONTRACT_CHECKS_IN_FRONT_END).  So only the
callee-side question is asked here, and the run-time-only parts of an entry
(a "caller" match, a "dynamic" output) are only validated.

*/

/* Header files common to all files. */
#include "fe_common.h"
#include "contract_config.h"

#ifdef PCH_PRAGMA_GUARD
/* Mark the end of the sequence of headers subject to precompiled header
   processing. */
#pragma hdrstop
#endif /* ifdef PCH_PRAGMA_GUARD */

/* Conditionally open the "edg" namespace. */
BEGIN_EDG_NAMESPACE

/*
The semantics a configuration can name: the four of
a_contract_evaluation_semantic, in the same order (see init_contract_config),
and those that the front end does not yet support, which are replaced by the
nearest supported one (see supported_semantic), as our GCC does when they are
not enabled.
*/
typedef enum a_config_semantic {
  cs_ignore,
  cs_observe,
  cs_enforce,
  cs_quick_enforce,
  cs_assume,		/* P3100; ignore is used. */
  cs_noexcept_enforce,	/* P4298; enforce is used. */
  cs_noexcept_observe	/* P4298; observe is used. */
} a_config_semantic;

/* The value of a_config_entry::kind for the "implicit" kind of P3100, which
   no contract assertion has yet. */
#define CONFIG_KIND_IMPLICIT  (-2)

/*
A source of configuration entries: an option, in command-line order.
*/
typedef struct a_config_source *a_config_source_ptr;
typedef struct a_config_source {
  a_config_source_ptr
		next;
  a_contract_config_source_kind
		kind;
  a_const_char	*arg;	/* The option's argument. */
} a_config_source;

/*
A range of line numbers in a "location" match.
*/
typedef struct a_line_range {
  unsigned long	start;
  unsigned long	end;
} a_line_range;

/*
A configuration entry.  A match criterion that is absent is -1 or NULL.
*/
typedef struct a_config_entry *a_config_entry_ptr;
typedef struct a_config_entry {
  a_config_entry_ptr
		next;
  int		kind;	/* An a_contract_kind, or CONFIG_KIND_IMPLICIT. */
  int		caller_side;
			/* 1 if the entry matches caller-side checks, 0 if
			   it matches callee-side ones; -1 (no "caller" key)
			   means callee-side ones too. */
  int		constexpr_eval;
			/* 1 if the entry matches only in constant
			   evaluation, 0 only outside it. */
  a_const_char	*group;	/* A group name, matched as a dot-separated
			   prefix. */
  a_const_char	*ns;	/* A namespace, matched as a "::"-separated
			   prefix. */
  a_const_char	*location_file;
			/* A file name, matched as a suffix that begins at
			   a path separator. */
  a_line_range	*location_lines;
			/* With location_file, the line ranges (or NULL for
			   any line), */
  unsigned long	num_location_lines;
			/* and their number. */
  unsigned long	location_column;
			/* With location_file, the column (0 for any). */
  a_boolean	has_semantic;
			/* TRUE if the output names a semantic. */
  a_config_semantic
		semantic;
  a_boolean	is_dynamic;
			/* TRUE if the output selects the semantic at run
			   time ("dynamic"). */
} a_config_entry;

/* The configuration sources, in command-line order. */
STATIC_THREAD a_config_source_ptr
		config_sources, last_config_source;

/* The configuration entries, in order; the last is the catch-all. */
STATIC_THREAD a_config_entry_ptr
		config_entries, last_config_entry;

/* TRUE once the configuration has asked for each semantic that is replaced
   (and been warned about). */
STATIC_THREAD a_boolean
		warned_noexcept_enforce, warned_noexcept_observe;

/* A buffer in which namespace names are built (see routine_namespace). */
STATIC_THREAD a_text_buffer_ptr
		namespace_buffer;


void add_contract_config_source(a_contract_config_source_kind  kind,
                                a_const_char                   *arg)
/*
Add a source of configuration entries of the given kind (the option arg is
the argument of) to the end of the list.  The sources are read by
init_contract_config.
*/
{
  a_config_source_ptr  srcp;

  /* alloc_general is called because the general mem_manage.c routines are
     not yet initialized (as in add_to_def_undef_list). */
  srcp = (a_config_source_ptr)alloc_general(sizeof(a_config_source));
  srcp->next = NULL;
  srcp->kind = kind;
  srcp->arg = arg;
  if (last_config_source != NULL) {
    last_config_source->next = srcp;
  } else {
    config_sources = srcp;
  }  /* if */
  last_config_source = srcp;
}  /* add_contract_config_source */


static char *copy_general_string(a_const_char  *str,
                                 sizeof_t      len)
/*
Return a null-terminated copy, in general memory, of the len characters at
str.
*/
{
  char  *result = alloc_general(len + 1);

  (void)memcpy(result, str, (size_t)len);
  result[len] = '\0';
  return result;
}  /* copy_general_string */


static a_boolean str_begins_with(a_const_char  *str,
                                 a_const_char  *prefix)
/*
Return TRUE if str begins with prefix.
*/
{
  return strncmp(str, prefix, strlen(prefix)) == 0;
}  /* str_begins_with */


/*
The JSON parser.  A JSON text is parsed into a tree of a_json_value entries,
in general memory, each with the line and column at which it begins (for
diagnostics).  Comments (// and block comments) are allowed, as in GCC.  The
first error ends the compilation.
*/
typedef enum a_json_kind {
  jk_null,
  jk_true,
  jk_false,
  jk_number,
  jk_string,
  jk_array,
  jk_object
} a_json_kind;

typedef struct a_json_value *a_json_value_ptr;
typedef struct a_json_value {
  a_json_kind	kind;
  unsigned long	line;
  unsigned long	column;
  a_const_char	*string;
			/* jk_string: the (unescaped) value; jk_number: its
			   text. */
  a_json_value_ptr
		elements;
			/* jk_array: the first element; jk_object: the value
			   of the first member. */
  a_json_value_ptr
		next;	/* The next element or member, if any. */
  a_const_char	*key;	/* For a member of an object, its key. */
} a_json_value;

/*
The state of the parse of one JSON text.
*/
typedef struct a_json_parser {
  a_const_char	*source;/* The text's description in diagnostics: the
			   file name, or "<command-line>". */
  a_const_char	*text;
  a_const_char	*p;	/* The next character. */
  unsigned long	line;	/* The line and column of *p. */
  unsigned long	column;
} a_json_parser;


static a_const_char *config_where(a_const_char   *source,
                                  unsigned long  line,
                                  unsigned long  column)
/*
Return "source:line:column", the position of something in a JSON text in a
diagnostic.
*/
{
  char  *where = alloc_general(strlen(source) + 50);

  (void)sprintf(where, "%s:%lu:%lu", source, line, column);
  return where;
}  /* config_where */


NORETURN static void config_error(a_const_char  *where,
                                  a_const_char  *message)
/*
Report the error message (in English: the configuration's own vocabulary),
found at where (see config_where), and end the compilation.
*/
{
  a_source_position  pos;

  set_position_to(pos, 0, SP_COL_CMD_LINE);
  pos_st2_diagnostic(es_command_line_error,
                     ec_cl_contract_configuration_error, &pos, where,
                     message);
  /* Avoid spurious warning.  The function above does not return. */
  exit_compilation(es_internal_error);
}  /* config_error */


static void config_warning(a_const_char  *where,
                           a_const_char  *message)
/*
Report the warning message, found at where (see config_where).
*/
{
  a_source_position  pos;

  set_position_to(pos, 0, SP_COL_CMD_LINE);
  pos_st2_diagnostic(es_command_line_warning,
                     ec_cl_contract_configuration_warning, &pos, where,
                     message);
}  /* config_warning */


NORETURN static void value_error(a_json_parser     *jp,
                                 a_json_value_ptr  value,
                                 a_const_char      *message)
/*
Report the error message at the JSON value, and end the compilation.
*/
{
  config_error(config_where(jp->source, value->line, value->column),
               message);
}  /* value_error */


NORETURN static void parse_error(a_json_parser  *jp,
                                 a_const_char   *message)
/*
Report the error message at the current character of the JSON text, and end
the compilation.
*/
{
  char  *full = alloc_general(strlen(message) + 40);

  (void)sprintf(full, "invalid JSON: %s", message);
  config_error(config_where(jp->source, jp->line, jp->column), full);
}  /* parse_error */


static void advance_char(a_json_parser  *jp)
/*
Advance over the current character of the JSON text.
*/
{
  if (*jp->p == '\n') {
    jp->line++;
    jp->column = 1;
  } else {
    jp->column++;
  }  /* if */
  jp->p++;
}  /* advance_char */


static void skip_json_white_space(a_json_parser  *jp)
/*
Skip white space and comments.
*/
{
  for (;;) {
    if (*jp->p == ' ' || *jp->p == '\t' || *jp->p == '\n' ||
        *jp->p == '\r') {
      advance_char(jp);
    } else if (jp->p[0] == '/' && jp->p[1] == '/') {
      while (*jp->p != '\0' && *jp->p != '\n') advance_char(jp);
    } else if (jp->p[0] == '/' && jp->p[1] == '*') {
      advance_char(jp);
      advance_char(jp);
      while (!(jp->p[0] == '*' && jp->p[1] == '/')) {
        if (*jp->p == '\0') parse_error(jp, "unterminated comment");
        advance_char(jp);
      }  /* while */
      advance_char(jp);
      advance_char(jp);
    } else {
      break;
    }  /* if */
  }  /* for */
}  /* skip_json_white_space */


static a_json_value_ptr alloc_json_value(a_json_parser  *jp,
                                         a_json_kind    kind)
/*
Allocate a JSON value of the given kind that begins at the current
character.
*/
{
  a_json_value_ptr  value =
                    (a_json_value_ptr)alloc_general(sizeof(a_json_value));

  value->kind = kind;
  value->line = jp->line;
  value->column = jp->column;
  value->string = NULL;
  value->elements = NULL;
  value->next = NULL;
  value->key = NULL;
  return value;
}  /* alloc_json_value */


static unsigned long scan_json_hex4(a_json_parser  *jp)
/*
Scan the four hexadecimal digits of a \u escape and return their value.
*/
{
  unsigned long  value = 0;
  int            i;

  for (i = 0; i < 4; i++) {
    char  c = *jp->p;
    value <<= 4;
    if (c >= '0' && c <= '9') {
      value += (unsigned long)(c - '0');
    } else if (c >= 'a' && c <= 'f') {
      value += (unsigned long)(c - 'a' + 10);
    } else if (c >= 'A' && c <= 'F') {
      value += (unsigned long)(c - 'A' + 10);
    } else {
      parse_error(jp, "invalid \\u escape");
    }  /* if */
    advance_char(jp);
  }  /* for */
  return value;
}  /* scan_json_hex4 */


static a_const_char *scan_json_string(a_json_parser  *jp)
/*
Scan a JSON string (the current character is its opening quote) and return
its value, null-terminated.  An escape gives at most as many bytes as it is
long, so the value fits in the length of its source.
*/
{
  a_const_char  *start;
  char          *result, *out;
  sizeof_t      max_len = 0;

  advance_char(jp);
  start = jp->p;
  while (start[max_len] != '"') {
    if (start[max_len] == '\0') parse_error(jp, "unterminated string");
    if (start[max_len] == '\\' && start[max_len + 1] != '\0') max_len++;
    max_len++;
  }  /* while */
  out = result = alloc_general(max_len + 1);
  while (*jp->p != '"') {
    unsigned char  c = (unsigned char)*jp->p;
    if (c < 0x20) parse_error(jp, "control character in string");
    if (c != '\\') {
      *out++ = (char)c;
      advance_char(jp);
      continue;
    }  /* if */
    advance_char(jp);
    c = (unsigned char)*jp->p;
    advance_char(jp);
    switch (c) {
      case '"':  *out++ = '"';  break;
      case '\\': *out++ = '\\'; break;
      case '/':  *out++ = '/';  break;
      case 'b':  *out++ = '\b'; break;
      case 'f':  *out++ = '\f'; break;
      case 'n':  *out++ = '\n'; break;
      case 'r':  *out++ = '\r'; break;
      case 't':  *out++ = '\t'; break;
      case 'u':
        { unsigned long  cp = scan_json_hex4(jp);
          if (cp >= 0xD800 && cp <= 0xDBFF && jp->p[0] == '\\' &&
              jp->p[1] == 'u') {
            /* A surrogate pair (12 characters give at most 4 bytes). */
            unsigned long  low;
            advance_char(jp);
            advance_char(jp);
            low = scan_json_hex4(jp);
            if (low < 0xDC00 || low > 0xDFFF) {
              parse_error(jp, "invalid surrogate pair");
            }  /* if */
            cp = 0x10000 + ((cp - 0xD800) << 10) + (low - 0xDC00);
          }  /* if */
          if (cp < 0x80) {
            *out++ = (char)cp;
          } else if (cp < 0x800) {
            *out++ = (char)(0xC0 | (cp >> 6));
            *out++ = (char)(0x80 | (cp & 0x3F));
          } else if (cp < 0x10000) {
            *out++ = (char)(0xE0 | (cp >> 12));
            *out++ = (char)(0x80 | ((cp >> 6) & 0x3F));
            *out++ = (char)(0x80 | (cp & 0x3F));
          } else {
            *out++ = (char)(0xF0 | (cp >> 18));
            *out++ = (char)(0x80 | ((cp >> 12) & 0x3F));
            *out++ = (char)(0x80 | ((cp >> 6) & 0x3F));
            *out++ = (char)(0x80 | (cp & 0x3F));
          }  /* if */
        }
        break;
      default:
        parse_error(jp, "invalid escape in string");
    }  /* switch */
  }  /* while */
  *out = '\0';
  advance_char(jp);
  return result;
}  /* scan_json_string */


static a_boolean is_json_digit(char  c)
/*
Return TRUE if c is a decimal digit.
*/
{
  return c >= '0' && c <= '9';
}  /* is_json_digit */


static a_json_value_ptr parse_json_value(a_json_parser  *jp);


static a_json_value_ptr parse_json_value(a_json_parser  *jp)
/*
Parse a JSON value at the current character (white space skipped).
*/
{
  a_json_value_ptr  value, *p_next;
  char              c = *jp->p;

  if (c == '{' || c == '[') {
    char  close = (c == '{') ? '}' : ']';
    value = alloc_json_value(jp, c == '{' ? jk_object : jk_array);
    p_next = &value->elements;
    advance_char(jp);
    skip_json_white_space(jp);
    if (*jp->p == close) {
      advance_char(jp);
      return value;
    }  /* if */
    for (;;) {
      a_const_char      *key = NULL;
      a_json_value_ptr  elem;
      if (c == '{') {
        if (*jp->p != '"') parse_error(jp, "expected a string key");
        key = scan_json_string(jp);
        skip_json_white_space(jp);
        if (*jp->p != ':') parse_error(jp, "expected ':'");
        advance_char(jp);
        skip_json_white_space(jp);
      }  /* if */
      elem = parse_json_value(jp);
      elem->key = key;
      *p_next = elem;
      p_next = &elem->next;
      skip_json_white_space(jp);
      if (*jp->p == ',') {
        advance_char(jp);
        skip_json_white_space(jp);
      } else if (*jp->p == close) {
        advance_char(jp);
        break;
      } else {
        parse_error(jp, c == '{' ? "expected ',' or '}'" :
                                   "expected ',' or ']'");
      }  /* if */
    }  /* for */
  } else if (c == '"') {
    value = alloc_json_value(jp, jk_string);
    value->string = scan_json_string(jp);
  } else if (c == '-' || is_json_digit(c)) {
    a_const_char  *start = jp->p;
    value = alloc_json_value(jp, jk_number);
    if (*jp->p == '-') advance_char(jp);
    if (*jp->p == '0') {
      advance_char(jp);
    } else if (is_json_digit(*jp->p)) {
      while (is_json_digit(*jp->p)) advance_char(jp);
    } else {
      parse_error(jp, "invalid number");
    }  /* if */
    if (*jp->p == '.') {
      advance_char(jp);
      if (!is_json_digit(*jp->p)) parse_error(jp, "invalid number");
      while (is_json_digit(*jp->p)) advance_char(jp);
    }  /* if */
    if (*jp->p == 'e' || *jp->p == 'E') {
      advance_char(jp);
      if (*jp->p == '+' || *jp->p == '-') advance_char(jp);
      if (!is_json_digit(*jp->p)) parse_error(jp, "invalid number");
      while (is_json_digit(*jp->p)) advance_char(jp);
    }  /* if */
    value->string = copy_general_string(start, (sizeof_t)(jp->p - start));
  } else if (str_begins_with(jp->p, "true")) {
    value = alloc_json_value(jp, jk_true);
    jp->p += 4;
    jp->column += 4;
  } else if (str_begins_with(jp->p, "false")) {
    value = alloc_json_value(jp, jk_false);
    jp->p += 5;
    jp->column += 5;
  } else if (str_begins_with(jp->p, "null")) {
    value = alloc_json_value(jp, jk_null);
    jp->p += 4;
    jp->column += 4;
  } else if (c == '\0') {
    parse_error(jp, "unexpected end of text");
  } else {
    parse_error(jp, "unexpected character");
  }  /* if */
  return value;
}  /* parse_json_value */


static a_json_value_ptr json_member(a_json_value_ptr  object,
                                    a_const_char      *key)
/*
Return the value of the member of the JSON object with the given key, or
NULL if there is none.  Of duplicate keys the last counts (as in GCC).
*/
{
  a_json_value_ptr  member, result = NULL;

  for (member = object->elements; member != NULL; member = member->next) {
    if (strcmp(member->key, key) == 0) result = member;
  }  /* for */
  return result;
}  /* json_member */


static a_boolean warn_unknown_keys(a_json_parser     *jp,
                                   a_json_value_ptr  object,
                                   a_const_char      *object_name,
                                   a_const_char      **known_keys)
/*
Warn about each key of the JSON object (the configuration's object_name
object) that is not in the NULL-terminated list known_keys.  Return TRUE if
there was one: The entry is then skipped.
*/
{
  a_json_value_ptr  member;
  a_boolean         unknown = FALSE;

  for (member = object->elements; member != NULL; member = member->next) {
    a_const_char  **k;
    for (k = known_keys; *k != NULL; k++) {
      if (strcmp(member->key, *k) == 0) break;
    }  /* for */
    if (*k == NULL) {
      char  *message = alloc_general(strlen(member->key) +
                                     strlen(object_name) + 60);
      (void)sprintf(message,
                    "unknown key \"%s\" in \"%s\" object; the entry is "
                    "skipped", member->key, object_name);
      config_warning(config_where(jp->source, member->line, member->column),
                     message);
      unknown = TRUE;
    }  /* if */
  }  /* for */
  return unknown;
}  /* warn_unknown_keys */


static a_boolean semantic_from_name(a_const_char       *name,
                                    a_config_semantic  *semantic)
/*
Set *semantic to the semantic with the given name and return TRUE, or return
FALSE if there is none.
*/
{
  if (strcmp(name, "ignore") == 0) {
    *semantic = cs_ignore;
  } else if (strcmp(name, "observe") == 0) {
    *semantic = cs_observe;
  } else if (strcmp(name, "enforce") == 0) {
    *semantic = cs_enforce;
  } else if (strcmp(name, "quick_enforce") == 0) {
    *semantic = cs_quick_enforce;
  } else if (strcmp(name, "assume") == 0) {
    *semantic = cs_assume;
  } else if (strcmp(name, "noexcept_enforce") == 0) {
    *semantic = cs_noexcept_enforce;
  } else if (strcmp(name, "noexcept_observe") == 0) {
    *semantic = cs_noexcept_observe;
  } else {
    return FALSE;
  }  /* if */
  return TRUE;
}  /* semantic_from_name */


static a_contract_evaluation_semantic supported_semantic(
                                              a_config_semantic  semantic)
/*
Return the evaluation semantic used for the configured semantic: itself, or
for one the front end does not yet support, the one our GCC uses when it is
not enabled (contract_semantic_best_fit): ignore for assume (without
-fcontracts-allow-assume), and the throwing variant for a noexcept one
(without -fcontracts-p4298).
*/
{
  a_contract_evaluation_semantic  result;

  switch (semantic) {
    case cs_ignore:
    case cs_assume:
      result = ces_ignore;
      break;
    case cs_observe:
    case cs_noexcept_observe:
      result = ces_observe;
      break;
    case cs_enforce:
    case cs_noexcept_enforce:
      result = ces_enforce;
      break;
    case cs_quick_enforce:
      result = ces_quick_enforce;
      break;
    default:
      unexpected_condition();
      result = ces_enforce;
  }  /* switch */
  return result;
}  /* supported_semantic */


static void check_entry_semantic(a_config_entry_ptr  entry,
                                 a_const_char        *where)
/*
Check the output of the configuration entry, read at where (see
config_where): Warn if it names a semantic that is replaced by another (once
per semantic, as GCC does), and in a configuration with only the ignore and
quick_enforce semantics (the C-generating back end, see the
--contract_evaluation_semantic option), report an error if it can give a
different one to a check the front end generates: one outside constant
evaluation, on the callee side (no caller-side checks are generated).
*/
{
  if (entry->has_semantic) {
    a_boolean  *p_warned = NULL;
    if (entry->semantic == cs_noexcept_enforce) {
      p_warned = &warned_noexcept_enforce;
    } else if (entry->semantic == cs_noexcept_observe) {
      p_warned = &warned_noexcept_observe;
    }  /* if */
    if (p_warned != NULL && !*p_warned) {
      *p_warned = TRUE;
      config_warning(where,
                     entry->semantic == cs_noexcept_enforce
                       ? "the configuration asks for \"noexcept_enforce\", "
                         "which is not supported; \"enforce\" is used "
                         "instead"
                       : "the configuration asks for \"noexcept_observe\", "
                         "which is not supported; \"observe\" is used "
                         "instead");
    }  /* if */
  }  /* if */
#if BACK_END_IS_C_GEN_BE
  if (entry->constexpr_eval != 1 && entry->caller_side != 1) {
    if (entry->is_dynamic) {
      config_error(where, "this configuration does not support a "
                          "\"dynamic\" output");
    }  /* if */
    if (entry->has_semantic) {
      a_contract_evaluation_semantic  semantic =
                                        supported_semantic(entry->semantic);
      if (semantic != ces_ignore && semantic != ces_quick_enforce) {
        /* See the --contract_evaluation_semantic option. */
      }  /* if */
    }  /* if */
  }  /* if */
#endif /* BACK_END_IS_C_GEN_BE */
}  /* check_entry_semantic */


static a_config_entry_ptr alloc_config_entry(void)
/*
Allocate a configuration entry that matches everything.
*/
{
  a_config_entry_ptr  entry =
                    (a_config_entry_ptr)alloc_general(sizeof(a_config_entry));

  entry->next = NULL;
  entry->kind = -1;
  entry->caller_side = -1;
  entry->constexpr_eval = -1;
  entry->group = NULL;
  entry->ns = NULL;
  entry->location_file = NULL;
  entry->location_lines = NULL;
  entry->num_location_lines = 0;
  entry->location_column = 0;
  entry->has_semantic = FALSE;
  entry->semantic = cs_enforce;
  entry->is_dynamic = FALSE;
  return entry;
}  /* alloc_config_entry */


static void append_config_entry(a_config_entry_ptr  entry)
/*
Add the configuration entry to the end of the list.
*/
{
  if (last_config_entry != NULL) {
    last_config_entry->next = entry;
  } else {
    config_entries = entry;
  }  /* if */
  last_config_entry = entry;
}  /* append_config_entry */


static void parse_location(a_json_parser     *jp,
                           a_json_value_ptr  value,
                           a_const_char      **p_file,
                           a_line_range      **p_lines,
                           unsigned long     *p_num_lines,
                           unsigned long     *p_column)
/*
Parse the "location" string value, "file", "file:ranges" or
"file:ranges:column" (ranges being "N" or "N-M" separated by commas), into
its parts.  A file name ending in ":digits" is read as ranges, as in GCC.
Report an error if the ranges are malformed.
*/
{
  a_const_char   *str = value->string, *last, *colon, *r;
  sizeof_t       end = (sizeof_t)strlen(str);
  unsigned long  n = 0;

  *p_file = str;
  *p_lines = NULL;
  *p_num_lines = 0;
  *p_column = 0;
  /* A trailing ":column" after a ":ranges" component. */
  last = strrchr(str, ':');
  if (last != NULL && last != str && is_json_digit(last[1]) &&
      last[1 + strspn(last + 1, "0123456789")] == '\0') {
    a_const_char  *prev = last - 1;
    while (prev > str && *prev != ':') --prev;
    if (*prev == ':' && prev + 1 < last && is_json_digit(prev[1]) &&
        prev + 1 + strspn(prev + 1, "0123456789-,") == last) {
      *p_column = strtoul(last + 1, (char **)NULL, 10);
      end = (sizeof_t)(last - str);
    }  /* if */
  }  /* if */
  /* The ranges, if any, follow the last colon before end. */
  colon = NULL;
  for (r = str; r < str + end; r++) {
    if (*r == ':') colon = r;
  }  /* for */
  if (colon == NULL || colon == str || !is_json_digit(colon[1])) {
    *p_file = copy_general_string(str, end);
    return;
  }  /* if */
  *p_file = copy_general_string(str, (sizeof_t)(colon - str));
  /* Count the ranges (at most one per comma, plus one). */
  for (r = colon + 1; r < str + end; r++) {
    if (*r == ',') n++;
  }  /* for */
  *p_lines = (a_line_range *)alloc_general((n + 1) * sizeof(a_line_range));
  r = colon + 1;
  while (r < str + end) {
    char           *after;
    unsigned long  start, finish;
    if (!is_json_digit(*r)) goto malformed;
    start = finish = strtoul(r, &after, 10);
    r = after;
    if (*r == '-') {
      r++;
      if (!is_json_digit(*r)) goto malformed;
      finish = strtoul(r, &after, 10);
      r = after;
    }  /* if */
    if (finish < start) goto malformed;
    (*p_lines)[*p_num_lines].start = start;
    (*p_lines)[*p_num_lines].end = finish;
    (*p_num_lines)++;
    if (r < str + end && *r == ',') {
      r++;
    } else if (r != str + end) {
      goto malformed;
    }  /* if */
  }  /* while */
  return;
malformed:
  { char  *message = alloc_general(strlen(str) + 50);
    (void)sprintf(message, "malformed line range \"%s\" in \"location\"",
                  str);
    value_error(jp, value, message);
  }
}  /* parse_location */


static a_const_char *required_string(a_json_parser     *jp,
                                     a_json_value_ptr  value,
                                     a_const_char      *message)
/*
Return the string value of the JSON value, or if it is not a string, report
the error message.
*/
{
  if (value->kind != jk_string) value_error(jp, value, message);
  return value->string;
}  /* required_string */


static int required_boolean(a_json_parser     *jp,
                            a_json_value_ptr  value,
                            a_const_char      *message)
/*
Return 1 or 0 for the JSON value true or false, or if it is neither, report
the error message.
*/
{
  if (value->kind == jk_true) return 1;
  if (value->kind != jk_false) value_error(jp, value, message);
  return 0;
}  /* required_boolean */


static a_boolean parse_config_output(a_json_parser       *jp,
                                     a_json_value_ptr    output,
                                     a_config_entry_ptr  entry)
/*
Parse the "output" object of a configuration entry into entry.  Return FALSE
if the entry is to be skipped (an unknown key).
*/
{
  static a_const_char  *known_output_keys[] =
                                     { "semantic", "dynamic", NULL };
  static a_const_char  *known_dynamic_keys[] =
                                  { "linkage", "name", "provideweak", NULL };
  a_json_value_ptr     value, dynamic;

  if (output->kind != jk_object) {
    value_error(jp, output, "\"output\" must be a JSON object");
  }  /* if */
  value = json_member(output, "semantic");
  if (value != NULL) {
    a_const_char  *name = required_string(jp, value,
                                          "\"semantic\" must be a string");
    if (!semantic_from_name(name, &entry->semantic)) {
      char  *message = alloc_general(strlen(name) + 50);
      (void)sprintf(message, "invalid contract evaluation semantic \"%s\"",
                    name);
      value_error(jp, value, message);
    }  /* if */
    entry->has_semantic = TRUE;
  }  /* if */
  dynamic = json_member(output, "dynamic");
  if (dynamic != NULL) {
    a_const_char      *name;
    a_boolean         cxx_linkage = TRUE;
    a_json_value_ptr  name_value;
    if (dynamic->kind != jk_object) {
      value_error(jp, dynamic, "\"dynamic\" must be a JSON object");
    }  /* if */
    name_value = json_member(dynamic, "name");
    if (name_value == NULL || name_value->kind != jk_string) {
      value_error(jp, dynamic, "\"dynamic\" requires a string \"name\"");
    }  /* if */
    name = name_value->string;
    value = json_member(dynamic, "linkage");
    if (value != NULL) {
      if (value->kind == jk_string && strcmp(value->string, "C++") == 0) {
        cxx_linkage = TRUE;
      } else if (value->kind == jk_string &&
                 strcmp(value->string, "C") == 0) {
        cxx_linkage = FALSE;
      } else {
        value_error(jp, value, "\"linkage\" must be \"C\" or \"C++\"");
      }  /* if */
    }  /* if */
    if (cxx_linkage) {
      /* A qualified name: a leading "::" names the global namespace, and
         any other empty component is an error. */
      sizeof_t  len;
      if (str_begins_with(name, "::")) name += 2;
      len = (sizeof_t)strlen(name);
      if (len == 0 || str_begins_with(name, "::") ||
          strstr(name, "::::") != NULL ||
          (len >= 2 && strcmp(name + len - 2, "::") == 0)) {
        value_error(jp, name_value,
                    "\"name\" in \"dynamic\" is not a valid qualified name");
      }  /* if */
    }  /* if */
    value = json_member(dynamic, "provideweak");
    if (value != NULL) {
      int  provideweak = required_boolean(jp, value,
                                       "\"provideweak\" must be a boolean");
      /* The weak definition returns the output's semantic. */
      if (provideweak && !entry->has_semantic) {
        value_error(jp, dynamic,
                    "\"provideweak\" requires an output \"semantic\"");
      }  /* if */
    }  /* if */
    if (warn_unknown_keys(jp, dynamic, "dynamic", known_dynamic_keys)) {
      return FALSE;
    }  /* if */
    entry->is_dynamic = TRUE;
  }  /* if */
  if (!entry->has_semantic && !entry->is_dynamic) {
    value_error(jp, output,
                "\"output\" requires a \"semantic\" or a \"dynamic\" field");
  }  /* if */
  return !warn_unknown_keys(jp, output, "output", known_output_keys);
}  /* parse_config_output */


static a_boolean parse_config_match(a_json_parser       *jp,
                                    a_json_value_ptr    match,
                                    a_config_entry_ptr  entry)
/*
Parse the "match" object of a configuration entry into entry.  Return FALSE
if the entry is to be skipped (an unknown key).
*/
{
  static a_const_char  *known_match_keys[] =
                       { "kind", "group", "caller", "constexpr", "namespace",
                         "location", NULL };
  static a_const_char  *known_caller_keys[] =
                       { "location", "namespace", NULL };
  a_json_value_ptr     value;

  if (match->kind != jk_object) {
    value_error(jp, match, "\"match\" must be a JSON object");
  }  /* if */
  value = json_member(match, "kind");
  if (value != NULL) {
    a_const_char  *kind = required_string(jp, value,
                                          "\"kind\" must be a string");
    if (strcmp(kind, "pre") == 0) {
      entry->kind = (int)ctk_pre;
    } else if (strcmp(kind, "post") == 0) {
      entry->kind = (int)ctk_post;
    } else if (strcmp(kind, "contract_assert") == 0) {
      entry->kind = (int)ctk_assert;
    } else if (strcmp(kind, "implicit") == 0) {
      entry->kind = CONFIG_KIND_IMPLICIT;
    } else {
      char  *message = alloc_general(strlen(kind) + 40);
      (void)sprintf(message, "invalid contract kind \"%s\"", kind);
      value_error(jp, value, message);
    }  /* if */
  }  /* if */
  value = json_member(match, "group");
  if (value != NULL) {
    entry->group = required_string(jp, value, "\"group\" must be a string");
  }  /* if */
  value = json_member(match, "caller");
  if (value != NULL) {
    if (value->kind == jk_true || value->kind == jk_false) {
      entry->caller_side = (value->kind == jk_true) ? 1 : 0;
    } else if (value->kind == jk_object) {
      /* The call site's location and namespace: validated only (see the
         comment at the top of the file). */
      a_json_value_ptr  member;
      entry->caller_side = 1;
      member = json_member(value, "location");
      if (member != NULL) {
        a_const_char   *file;
        a_line_range   *lines;
        unsigned long  num_lines, column;
        (void)required_string(jp, member,
                              "\"location\" in \"caller\" must be a string");
        parse_location(jp, member, &file, &lines, &num_lines, &column);
      }  /* if */
      member = json_member(value, "namespace");
      if (member != NULL) {
        a_const_char  *ns = required_string(jp, member,
                             "\"namespace\" in \"caller\" must be a string");
        if (*ns == '\0') {
          value_error(jp, member,
                      "\"namespace\" in \"caller\" must not be empty");
        }  /* if */
      }  /* if */
      if (warn_unknown_keys(jp, value, "caller", known_caller_keys)) {
        return FALSE;
      }  /* if */
    } else {
      value_error(jp, value, "\"caller\" must be a boolean or object");
    }  /* if */
  }  /* if */
  value = json_member(match, "constexpr");
  if (value != NULL) {
    entry->constexpr_eval = required_boolean(jp, value,
                                         "\"constexpr\" must be a boolean");
  }  /* if */
  value = json_member(match, "namespace");
  if (value != NULL) {
    entry->ns = required_string(jp, value, "\"namespace\" must be a string");
    if (*entry->ns == '\0') {
      value_error(jp, value, "\"namespace\" must not be empty");
    }  /* if */
  }  /* if */
  value = json_member(match, "location");
  if (value != NULL) {
    (void)required_string(jp, value, "\"location\" must be a string");
    parse_location(jp, value, &entry->location_file,
                         &entry->location_lines, &entry->num_location_lines,
                         &entry->location_column);
  }  /* if */
  return !warn_unknown_keys(jp, match, "match", known_match_keys);
}  /* parse_config_match */


static void parse_config_json(a_const_char  *text,
                              a_const_char  *source)
/*
Parse the JSON configuration text (described in diagnostics as source) and
append its entries to the list.
*/
{
  a_json_parser     jp;
  a_json_value_ptr  root, elem;

  jp.source = source;
  jp.text = jp.p = text;
  jp.line = jp.column = 1;
  skip_json_white_space(&jp);
  root = parse_json_value(&jp);
  skip_json_white_space(&jp);
  if (*jp.p != '\0') parse_error(&jp, "text after the end of the value");
  if (root->kind != jk_array) {
    value_error(&jp, root, "contract configuration must be a JSON array");
  }  /* if */
  for (elem = root->elements; elem != NULL; elem = elem->next) {
    a_config_entry_ptr  entry;
    a_json_value_ptr    output, match;
    if (elem->kind != jk_object) {
      value_error(&jp, elem,
                  "contract configuration entry must be a JSON object");
    }  /* if */
    output = json_member(elem, "output");
    if (output == NULL) {
      value_error(&jp, elem, "contract configuration entry is missing "
                             "required \"output\" field");
    }  /* if */
    entry = alloc_config_entry();
    if (!parse_config_output(&jp, output, entry)) continue;
    match = json_member(elem, "match");
    if (match != NULL && !parse_config_match(&jp, match, entry)) continue;
    check_entry_semantic(entry, config_where(source, elem->line,
                                             elem->column));
    append_config_entry(entry);
  }  /* for */
}  /* parse_config_json */


static void parse_config_file(a_const_char  *file_name)
/*
Parse the JSON configuration file file_name and append its entries to the
list.
*/
{
  FILE      *f = fopen(file_name, "rb");
  char      *text;
  sizeof_t  size = 0, allocated = 4096;

  if (f == NULL) {
    str_command_line_error(ec_cl_contract_configuration_unreadable,
                           file_name);
  }  /* if */
  text = alloc_general(allocated);
  for (;;) {
    size_t  n = fread(text + size, 1, (size_t)(allocated - size - 1), f);
    size += (sizeof_t)n;
    if (size + 1 < allocated) break;
    { /* Full: Double the buffer. */
      char  *bigger = alloc_general(allocated * 2);
      (void)memcpy(bigger, text, (size_t)size);
      text = bigger;
      allocated *= 2;
    }
  }  /* for */
  if (ferror(f)) {
    str_command_line_error(ec_cl_contract_configuration_unreadable,
                           file_name);
  }  /* if */
  (void)fclose(f);
  text[size] = '\0';
  parse_config_json(text, file_name);
}  /* parse_config_file */


static a_boolean group_name_is_valid(a_const_char  *name,
                                     sizeof_t      len)
/*
Return TRUE if the len characters at name are a valid contract group name:
letters, digits, '_', '-', '.' and ':' (P3400 names are dot-separated, P3100
names begin "ub:"), with no empty dot-separated component.
*/
{
  sizeof_t  i;

  if (len == 0 || name[0] == '.' || name[len - 1] == '.') return FALSE;
  for (i = 0; i < len; i++) {
    char  c = name[i];
    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
          is_json_digit(c) || c == '_' || c == '-' || c == '.' || c == ':') ||
        (c == '.' && i + 1 < len && name[i + 1] == '.')) {
      return FALSE;
    }  /* if */
  }  /* for */
  return TRUE;
}  /* group_name_is_valid */


static void parse_group_semantics(a_const_char  *arg)
/*
Append an entry for each element of arg, the argument of a
--contract_group_evaluation_semantic option: a comma-separated list of
group:semantic, the semantic following the last colon (a P3100 group name
contains one itself).  Such an entry matches only callee-side checks.
*/
{
  a_const_char  *elt = arg;

  for (;;) {
    a_const_char        *comma = strchr(elt, ',');
    sizeof_t            len = comma != NULL ? (sizeof_t)(comma - elt)
                                                : (sizeof_t)strlen(elt);
    char                *str = copy_general_string(elt, len);
    char                *colon = strrchr(str, ':');
    char                *where = alloc_general(len + 60);
    a_config_entry_ptr  entry;
    (void)sprintf(where, "--contract_group_evaluation_semantic=%s",
                  str);
    if (colon == NULL || colon == str || colon[1] == '\0') {
      config_error(where, "expected group:semantic");
    }  /* if */
    if (!group_name_is_valid(str, (sizeof_t)(colon - str))) {
      config_error(where, "invalid group name");
    }  /* if */
    entry = alloc_config_entry();
    if (!semantic_from_name(colon + 1, &entry->semantic)) {
      config_error(where, "invalid contract evaluation semantic");
    }  /* if */
    entry->has_semantic = TRUE;
    entry->caller_side = 0;
    *colon = '\0';
    entry->group = str;
    check_entry_semantic(entry, where);
    append_config_entry(entry);
    if (comma == NULL) break;
    elt = comma + 1;
  }  /* for */
}  /* parse_group_semantics */


void init_contract_config(void)
/*
Build the list of configuration entries from the configuration sources (see
add_contract_config_source), in order, and a catch-all for the
--contract_evaluation_semantic option.  Called once the command line is
processed.
*/
{
  a_config_source_ptr  srcp;
  a_config_entry_ptr   entry;

  config_entries = last_config_entry = NULL;
  if (config_sources != NULL && !contracts_enabled) {
    str_command_line_warning(ec_cl_contract_configuration_without_contracts,
                             (a_const_char *)NULL);
  }  /* if */
  if (!contracts_enabled) return;
  for (srcp = config_sources; srcp != NULL; srcp = srcp->next) {
    switch (srcp->kind) {
      case ccsk_group_semantic:
        parse_group_semantics(srcp->arg);
        break;
      case ccsk_json_inline:
        parse_config_json(srcp->arg, "<command-line>");
        break;
      case ccsk_json_file:
        parse_config_file(srcp->arg);
        break;
      default:
        unexpected_condition();
    }  /* switch */
  }  /* for */
  entry = alloc_config_entry();
  entry->has_semantic = TRUE;
  /* The first four semantics are in the same order in both types. */
  entry->semantic = (a_config_semantic)contract_evaluation_semantic;
  append_config_entry(entry);
}  /* init_contract_config */


static a_boolean namespace_matches(a_const_char  *entry_ns,
                                   a_const_char  *ns)
/*
Return TRUE if the namespace ns ("" for the global namespace) is entry_ns or
nested in it.
*/
{
  sizeof_t  len = (sizeof_t)strlen(entry_ns);

  if (strncmp(ns, entry_ns, (size_t)len) != 0) return FALSE;
  return ns[len] == '\0' || (ns[len] == ':' && ns[len + 1] == ':');
}  /* namespace_matches */


static a_boolean file_name_suffix_matches(a_const_char  *entry_file,
                                          a_const_char  *file_name)
/*
Return TRUE if file_name ends with entry_file, beginning at a path separator
or at its start.
*/
{
  sizeof_t  elen = (sizeof_t)strlen(entry_file);
  sizeof_t  flen = (sizeof_t)strlen(file_name);
  char      before;

  if (elen > flen) return FALSE;
  if (strcmp(file_name + flen - elen, entry_file) != 0) return FALSE;
  if (elen == flen) return TRUE;
  before = file_name[flen - elen - 1];
  return before == '/' || before == '\\';
}  /* file_name_suffix_matches */


static void add_namespace_name(a_scope_ptr  scope)
/*
Add to namespace_buffer the qualified name of the namespace whose scope is
scope (nothing for the global namespace).  An unnamed namespace contributes
no component, as in GCC.
*/
{
  a_const_char  *name;

  if (scope == NULL || scope->kind != (a_scope_kind)sck_namespace) return;
  add_namespace_name(scope->parent);
  name = scope->variant.assoc_namespace->source_corresp.name;
  if (name == NULL) return;
  if (namespace_buffer->size != 0) {
    add_string_to_text_buffer(namespace_buffer, "::");
  }  /* if */
  add_string_to_text_buffer(namespace_buffer, name);
}  /* add_namespace_name */


static a_const_char *routine_namespace(a_routine_ptr  routine)
/*
Return the qualified name of the namespace enclosing routine ("" for the
global namespace), looking out through classes, and through functions for
local classes and lambdas.  The name is in namespace_buffer, valid until the
next call.
*/
{
  a_source_correspondence_ptr  scp = &routine->source_corresp;
  a_scope_ptr                  scope = scp->parent_scope;

  if (namespace_buffer == NULL) namespace_buffer = alloc_text_buffer(64);
  reset_text_buffer(namespace_buffer);
  for (;;) {
    if (scope == NULL) {
      /* In file scope memory with a function or block scope parent: Look
         from the enclosing routine. */
      if (scp == NULL || scp->enclosing_routine == NULL) break;
      scp = &scp->enclosing_routine->source_corresp;
      scope = scp->parent_scope;
    } else if (scope->kind == (a_scope_kind)sck_namespace) {
      add_namespace_name(scope);
      break;
    } else if (scope->kind == (a_scope_kind)sck_function) {
      scp = &scope->variant.routine.ptr->source_corresp;
      scope = scp->parent_scope;
    } else if (scope->kind == (a_scope_kind)sck_class_struct_union) {
      scp = &scope->variant.assoc_type->source_corresp;
      scope = scp->parent_scope;
    } else if (scope->kind == (a_scope_kind)sck_file) {
      break;
    } else {
      /* A block scope, e.g. */
      scp = NULL;
      scope = scope->parent;
    }  /* if */
  }  /* for */
  add_to_text_buffer(namespace_buffer, "", (sizeof_t)1);
  return namespace_buffer->buffer;
}  /* routine_namespace */


static a_boolean config_entry_matches(a_config_entry_ptr        entry,
                                      a_contract_specifier_ptr  csp,
                                      a_routine_ptr             routine,
                                      a_boolean                 in_ce)
/*
Return TRUE if the configuration entry matches the callee-side check of the
contract assertion csp, in routine (NULL if unknown), in constant evaluation
if in_ce is TRUE.
*/
{
  if (entry->kind != -1 && entry->kind != (int)csp->kind) return FALSE;
  /* Only the callee-side question is asked (see the top of the file). */
  if (entry->caller_side == 1) return FALSE;
  if (entry->constexpr_eval != -1 &&
      entry->constexpr_eval != (in_ce ? 1 : 0)) {
    return FALSE;
  }  /* if */
  if (entry->ns != NULL &&
      !namespace_matches(entry->ns, routine != NULL
                                      ? routine_namespace(routine) : "")) {
    return FALSE;
  }  /* if */
  if (entry->location_file != NULL) {
    /* The presumed position (that of #line directives), as in GCC. */
    a_line_number      line = 0;
    a_boolean          at_end;
    a_source_file_ptr  sfp = NULL;
    if (csp->position.seq != 0) {
      sfp = source_file_for_seq(csp->position.seq, &line, &at_end,
                                /*physical_line=*/FALSE);
    }  /* if */
    if (sfp == NULL ||
        !file_name_suffix_matches(entry->location_file, sfp->file_name)) {
      return FALSE;
    }  /* if */
    if (entry->location_lines != NULL) {
      unsigned long  i;
      for (i = 0; i < entry->num_location_lines; i++) {
        if ((unsigned long)line >= entry->location_lines[i].start &&
            (unsigned long)line <= entry->location_lines[i].end) {
          break;
        }  /* if */
      }  /* for */
      if (i == entry->num_location_lines) return FALSE;
    }  /* if */
    if (entry->location_column != 0 &&
        (unsigned long)csp->position.column != entry->location_column) {
      return FALSE;
    }  /* if */
  }  /* if */
  return TRUE;
}  /* config_entry_matches */


a_contract_evaluation_semantic contract_semantic_for(
                                        a_contract_specifier_ptr  csp,
                                        a_routine_ptr             routine,
                                        a_boolean                 in_ce)
/*
Return the evaluation semantic of the contract assertion csp, of routine
(NULL if unknown; it gives the namespace), in constant evaluation if in_ce is
TRUE, at run time otherwise: that of the first
configuration entry that matches.  In constant evaluation an entry that
selects the semantic at run time and names no semantic of its own is
passed over (P3595); with a semantic of its own, that is used.  The facets
of a label (P3400) adjust it (see apply_contract_label_facets).
*/
{
  a_config_entry_ptr  entry;
  a_contract_evaluation_semantic
                      semantic = contract_evaluation_semantic;

  for (entry = config_entries; entry != NULL; entry = entry->next) {
    if (!config_entry_matches(entry, csp, routine, in_ce)) {
      continue;
    }  /* if */
    if (!entry->has_semantic) {
      /* Only a dynamic entry has no semantic.  At run time its semantic is
         chosen by g++'s code, and only constant evaluation and C-generating
         checks (for which a dynamic entry is an error) ask here. */
      check_assertion(entry->is_dynamic);
      continue;
    }  /* if */
    semantic = supported_semantic(entry->semantic);
    break;
  }  /* for */
  /* Otherwise the default: before the configuration is built, or without
     one. */
  if (csp->label != NULL &&
      (csp->label_allowed_semantics != 0 ||
       csp->label_computed_semantics[1] != 0)) {
    semantic = apply_contract_label_facets(csp, semantic, in_ce);
  }  /* if */
  return semantic;
}  /* contract_semantic_for */


a_boolean contract_semantic_possible(a_contract_evaluation_semantic  semantic)
/*
Return TRUE if some contract assertion can have the evaluation semantic
outside constant evaluation (as far as the configuration tells).
*/
{
  a_config_entry_ptr  entry;

  if (config_entries == NULL) return contract_evaluation_semantic == semantic;
  for (entry = config_entries; entry != NULL; entry = entry->next) {
    if (entry->has_semantic && entry->constexpr_eval != 1 &&
        entry->caller_side != 1 &&
        supported_semantic(entry->semantic) == semantic) {
      return TRUE;
    }  /* if */
  }  /* for */
  return FALSE;
}  /* contract_semantic_possible */

/* Conditionally close the "edg" namespace. */
END_EDG_NAMESPACE
