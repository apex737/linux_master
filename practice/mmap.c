#include "../def.h"
#include <sys/mman.h>

#define CHECK_MAP_SUCCESS(_addr) \
if(_addr == MAP_FAILED) error_handler("mmap") \

#define PRINT_ADDRS(msg)                \
{                                       \
    printf("%s\n", msg);                \
    printf("addr1 = %s\n", addr1);      \
    printf("addr2 = %s\n", addr2);      \
}                                       \



int main(int argc, char* argv[])
{
    caddr_t addr1, addr2;
    char fileName[256] = "input.dat";
    // argv[1]을 fileName으로 새로 할당
    if (argc != 2){
        printf("USAGE: %s <filename>\n", argv[0]);
        exit(1);
    } 
    
    strcpy(fileName, argv[1]);
    int fd = open(fileName, O_RDWR, 0644);
    if(fd == -1) error_handler("open");
    int pageSize = getpagesize();
    printf("pageSz: %d\n", pageSize);
    // 파일을 pageSize만큼 확장합니다.
    // if (ftruncate(fd, pageSize) == -1) error_handler("ftruncate");
    addr1 = mmap(
        NULL,                       // OS addr 자동할당
        pageSize,                   // 매핑할 크기
        PROT_READ | PROT_WRITE,     // 접근 권한: open 권한과 맞춰야함
        MAP_SHARED,                 // 매핑된 메모리 영역을 멀티 프로세스와 공유
        fd,                         // 열린 fd
        0                           // 매핑 시작 위치 
    );
    CHECK_MAP_SUCCESS(addr1);
    addr2 = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    CHECK_MAP_SUCCESS(addr2);

    close(fd);

    printf("%s", addr1);
    addr1[0] = '1'; PRINT_ADDRS("After addr1[0]=1");
    addr2[0] = '2'; PRINT_ADDRS("After addr2[0]=2");
    addr1[0] = '3'; PRINT_ADDRS("After addr1[0]=3");

    execlp("cat", "cat", fileName, NULL);
    return 0;
}