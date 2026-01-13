#include "def.h"
#include <errno.h>
#include <sys/file.h>

int main(int argc, const char** argv)
{
    // flock(fd, option), return
    int option;
    if(argc < 2)    {
        printf("usage %s sh|ex\n", argv[0]);
        return -1;
    } else if (strcmp(argv[1], "sh") == 0) {
        option = LOCK_SH;
    } else if (strcmp(argv[1], "ex") == 0) {
        option = LOCK_EX;
    } else {
        printf("usage %s sh|ex\n", argv[0]);
        return -1;
    }

    int fd = open("lock_test", O_CREAT | O_TRUNC | O_RDWR, 0644);
    if(fd == -1)
    {
        puts("open(failed)");
        return -1;
    }

    puts("Trying to get the Lock");
    
    if(flock(fd, option) == -1)
    {
        puts("flock() failed");
        printf("option: %d, errno: %d (%s)\n", option, errno, strerror(errno));
        goto err;
    }
    getchar();
    puts("Grabbed the Lock");

    close(fd);

    return 0;
err:
    close(fd);
    return -1;
}
