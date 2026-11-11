//remark:prefix maps: through edg-gxx -fmacro-prefix-map= and -ffile-prefix-map= remap __FILE__ as g++ does: every -ffile-prefix-map= is tried before any -fmacro-prefix-map=, whatever their order, the last given of each first; under -fcanon-prefix-map OLD and a file name with a directory are compared with ".", ".." and links resolved
//require:BACK_END_IS_CP_GEN_BE 1
//options_sep:!
//options:-fmacro-prefix-map==MACRO/ -ffile-prefix-map==FILE/;rp!-ffile-prefix-map==FILE/ -fmacro-prefix-map==MACRO/;rp!-fmacro-prefix-map==FIRST/ -fmacro-prefix-map==SECOND/;rp!-fmacro-prefix-map=./=/mapped;rp!-fcanon-prefix-map -fmacro-prefix-map=./=/mapped;rp!-fcanon-prefix-map -fno-canon-prefix-map -fmacro-prefix-map=./=/mapped;rp
// The second line's #line name has a directory, "./.", which the canonical
// map resolves to the directory of the test: g++ itself gives the same
// results for the same options.
extern "C" int puts(const char *);

int main() {
  puts(__FILE__);
#line 100 "././Test_name.c"
  puts(__FILE__);
  return 0;
}
