//remark:prefix maps: --macro_prefix_map=OLD=NEW remaps the file name that __FILE__, __BASE_FILE__ and __FILE_NAME__ yield when it begins with OLD, the last map given tried first, in a #line name too; one that does not match leaves the name alone, and an argument without "=" is an error
//options_sep:!
//options:--macro_prefix_map =MAPPED/;rp!--macro_prefix_map =FIRST/ --macro_prefix_map =SECOND/;rp!--macro_prefix_map nomatch=MAPPED/;rp!--macro_prefix_map =FIRST/ --macro_prefix_map T=SECOND/;rp!--macro_prefix_map nonsense;fc
// (No system headers: also runs under the C-generating back end.)
// Mirrors: gcc/testsuite/c-c++-common/cpp/fmacro-prefix-map.c and
// ffile-prefix-map.c (gnu_gcc).  __FILE_NAME__ exists only in GNU and Clang
// modes.
extern "C" int puts(const char *);

int main() {
  puts(__FILE__);
  puts(__BASE_FILE__);
#ifdef __FILE_NAME__
  puts(__FILE_NAME__);
#endif
#line 100 "dir/renamed.cpp"
  puts(__FILE__);
#ifdef __FILE_NAME__
  puts(__FILE_NAME__);
#endif
  return 0;
}
