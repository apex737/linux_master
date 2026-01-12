#include "def.h"
#include <sys/stat.h>

#define TARGET_FILE    "fp1.c"
int main()
{
  link(TARGET_FILE, "hardlink");
  symlink(TARGET_FILE, "softlink");

  return 0;
}
