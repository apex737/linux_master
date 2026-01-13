#include "def.h"
#include <errno.h>
#include <sys/file.h>

/*  임계구역: Race Condition을 일으킬 수 있는 코드영역 (리소스가 아님)
    flock(fd, Lock Option Flags);
    LOCK_EX: "파일 Write할거임, 접근 금지"	(획득자 유일)
    LOCK_SH: "파일 Read 할거임, Write 금지" (복수 획득자 가능)
    LOCK_NB: Lock 획득에 실패하면 대기하지 않고 에러 (-1) 리턴
    * Issue; Lock 실패로 리턴한건지, fd 접근이 실패한건지 모름
        => if (errno == EWOULDBLOCK) 확인필수;
        EWOULDBLOCK: "지금 작업하면 블로킹 될 상황이라서 일단 멈췄어"라는 뜻의 에러 코드
*/
int main(int argc, const char** argv)
{
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
    if(flock(fd, option | LOCK_NB) == -1)
    {
        puts("flock() failed");
        printf("option: %d, errno: %d (%s)\n", option, errno, strerror(errno));
        goto err;
    }
    puts("Grabbed the Lock");
    getchar();
    flock(fd, LOCK_UN);
    puts("Released the Lock");
    close(fd);

    return 0;
err:
    close(fd);
    return -1;
}
