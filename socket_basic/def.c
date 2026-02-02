#include "def.h"

void error_handler(char* msg)
{
    perror(msg);
    exit(1);
}

// readn by Richard Stevens
ssize_t readn(int fd, void* vptr, size_t n)
{
    ssize_t nread;
    size_t nleft = n;
    char* ptr = vptr;
    while(nleft > 0)
    {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR) nread = 0; // 인터럽트 발생 시 무시
            else return -1;
        } else if (nread == 0) break; // EOF (연결 종료)

        nleft -= nread; // 남은 바이트 수 계산
        ptr += nread;   // 읽은 만큼 포인터 이동
    }
    return (n - nleft); // 실제로 읽은 총 바이트 수 반환
}