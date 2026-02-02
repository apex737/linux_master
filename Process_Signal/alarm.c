#include "../def.h"
#include <signal.h>
#include <sys/time.h>

/*  시그널 생애주기 (발생 -> 펜딩 -> 처리) */


void sigalrm_handler(int signum)
{
    // psignal(signum, "sigalrm");
    printf("Alarm %s\n", strsignal(signum) );
}

int main()
{
    // 커스텀 시그널 핸들러 등록 
    signal(SIGALRM, sigalrm_handler); 
    // 1초 후 첫 트리거 이후, 5초에 1회 SIGALRM 발생
    struct itimerval tim = {{5,0}, {1,0}}; 
    // 커널 모드로 진입: setitimer 호출
    setitimer(ITIMER_REAL, &tim, NULL);
    // 유저 모드로 복귀
   
    while(1) pause();   
    /*  1. 커널 모드로 진입: 
            - pause() 호출; 프로세스가 실행 큐 => 대기 큐로 이동
        2. 시그널 발생
            - 커널이 타겟 프로세스의 task_struct의 SIGALRM pending 비트를 set
            - 커널이 타겟 프로세스를 Wait => Run으로 상태 변경
        3. 펜딩 비트 체크
            - 프로세스가 Run으로 가기 전에 pending bit 체크
            - SIGALRM이 set 되어있으므로 유저 모드 복귀 직후 sigalrm_handler 실행
            - 유저 코드 pause() 호출; 프로세스가 실행 큐 => 대기 큐로 이동   */
    return 0;
}