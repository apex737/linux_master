#include <stdio.h>
#include <string.h>
#include <fcntl.h>

// O_CREAT, O_DIRECTORY,  O_EXCL,  O_NOCTTY,  
// O_NOFOLLOW,  O_TMPFILE, O_TRUNC
void write_file()
{
    open("write", O_WRONLY | OCREAT, 0644)

}

int main()
{
    

    return 1;
}
