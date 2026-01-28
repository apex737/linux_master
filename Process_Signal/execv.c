#define _GNU_SOURCE  // execvpe를 위해 GNU 확장 사용 선언 (반드시 맨 위에!)
#include "../def.h"

int main()
{
    puts("Before Exec");
    // 주로 char* []에 값을 받아서 실행하게됨
    // main의 argv 인자가 바로 여기에 해당함
    char *argv[] = {"env", NULL};
    // main의 envp 인자가 바로 여기에 해당함
    char *envp[] = {"MY_NAME=뚱인데요?", NULL};
    // p -> PATH (환경변수) 참조 가능
    // e -> 환경변수 추가 
    execvpe("env", argv, envp);
    // 실행 안됨
    printf("MY NAME: %s\n", getenv("MY_NAME"));
    puts("After Exec"); 
    return 0;
}