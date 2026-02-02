#include "../def.h"
#include <sys/mman.h>

#define CHECK_MMAP(addr) \
if(addr == MAP_FAILED) error_handler("mmap"); \

int main(int argc, char* argv[])
{
    int fd;
    char* addr;
    char fileName[256] = "map.dat";
    if(argc != 2){
        printf("USAGE: %s <filename>\n", argv[0]);
        exit(1);
    }
    if((fd = open(argv[1], O_RDWR)) == -1) error_handler("open");
    int pageSize = getpagesize();
    addr = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    CHECK_MMAP(addr);
    close(fd);
    char c = 'a';
    while(1)
    {
        sleep(2);
        addr[0] = c++;
        if(c >'z') c = 'a';
    }
    return 0;
}