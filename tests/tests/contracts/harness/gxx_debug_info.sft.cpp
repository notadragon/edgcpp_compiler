//remark:contracts: through edg-gxx, -g debug information names the source as g++ alone does (as it was given), not the temporary generated C++ file, so it is the same from run to run
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options:-g:-g -ffile-prefix-map=Test_name=Mapped_name
// The program reads its own debug information.  edg_gxx.sh gives g++ a
// debug prefix map from each generated file's name to its source's, after
// the maps given (the second case's turns Test_name.c into Mapped_name.c,
// as g++ alone would).
#include <cstdio>
#include <cstring>
#include <unistd.h>

int main()
{
  char cmd[64];
  snprintf(cmd, sizeof cmd, "readelf --debug-dump=info /proc/%d/exe",
           (int)getpid());
  FILE *p = popen(cmd, "r");
  if (p == nullptr) return 1;
  char line[4096];
  int generated = 0, source = 0, mapped = 0;
  while (fgets(line, sizeof line, p) != nullptr) {
    if (strstr(line, "DW_AT_name") == nullptr) continue;
    if (strstr(line, "/edg-gxx.") != nullptr) ++generated;
    else if (strstr(line, "Test_name.c\n") != nullptr) ++source;
    else if (strstr(line, "Mapped_name.c\n") != nullptr) ++mapped;
  }
  pclose(p);
  printf("compilation unit named after %s\n",
         generated ? "the generated file"
         : source ? "the source"
         : mapped ? "the source, mapped" : "nothing");
  return 0;
}
