#include "def.h"
#include <dirent.h>
#include <sys/types.h>

// MACRO Method 
#define TYPE2STR(X)\
    ((X)==DT_BLK    ? "This is block " : \
     (X)==DT_CHR   ? "This is character " : \
     (X)==DT_DIR   ? "This is directory " : \
     (X)==DT_FIFO  ? "This is fifo " : \
     (X)==DT_LNK   ? "This is link " : \
     (X)==DT_REG   ? "This is regular " : \
     (X)==DT_SOCK  ? "This is socket " : \
     (X)==DT_UNKNOWN ? "Unknown " : \
    "unknown") 
    
// Helper (Inline) Function
static inline const char* type_to_str(unsigned char x)
{
  switch(x)
  {
    DT_BLK     : return "This is block ";    
    DT_CHR     : return "This is character ";
    DT_DIR     : return "This is directory ";
    DT_FIFO    : return "This is fifo ";
    DT_LNK     : return "This is link ";
    DT_REG     : return "This is regular ";
    DT_SOCK    : return "This is socket ";
    DT_UNKNOWN : return "This is unknown ";
    default    : return "This is unknown ";
  }
}

int main()
{
    // open current directory
    DIR* dp = opendir(".");
    // define current entry ptr 
    struct dirent* pEntry = NULL; 
    // loop internal dir until meet an EOF 
    // Issue: NULL can be an EOF or Unaccessable file

    while(pEntry = readdir(dp))
    {
      if(pEntry)
      {
        printf("type: %s , name: %s\n",
            TYPE2STR(pEntry->d_type), pEntry->d_name);
      }
      // move to next fp
      
    }

    closedir(dp);
    
    return 0;
}
