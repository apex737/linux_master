#include "../def.h"
#include <sys/mman.h>

/*  mmap IPC
    mmap의 MAP_SHARED 옵션을 사용하여
    Page Cache의 파일 영역을 IPC를 위한 버퍼로 공유한다. */
    
#define CHECK_MMAP(addr) \
    if(addr == MAP_FAILED) error_handler("mmap"); \

int main(int argc, char* argv[])
{
    if(argc!=2){
        printf("USAGE: %s <filename>\n", argv[0]);
        exit(1);
    }
    char fileName[256] = "input.dat"; 
    strcpy(fileName, argv[1]);
    int fd = open(fileName, O_RDWR);
    if(fd==-1) error_handler("open");
    int pageSize = getpagesize();
    // fd (pageSize 크기의 공유버퍼)로 IPC
    caddr_t addr = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    CHECK_MMAP(addr);
    close(fd);

    int pid;
    switch (pid = fork())
    {
    case -1:
        error_handler("fork");
        break;
    case 0:
        printf("1. Child Proccess: addr=%s", addr); sleep(1);
        addr[0] = 'x';
        printf("2. Child Proccess: addr=%s", addr); sleep(2);
        printf("3. Child Proccess: addr=%s", addr);
        break;
    default:
        printf("1. Parent Proccess: addr=%s", addr); sleep(2);
        printf("2. Parent Proccess: addr=%s", addr);
        addr[1] = 'y';
        printf("3. Parent Proccess: addr=%s", addr);
        break;
    break;
    }
    return 0;
}