#include "../def.h"

int main()
{
    puts("Before Exec");
    // 기존 프로세스를 뺏어서 ls -a 실행
    // 첫번째 인자가 참조자이고, 두번째 인자는 프로그램 이름 (참조X)    
    execlp("ls", "ls", "-a", (char*)NULL);
    // 실행 안됨
    puts("After Exec"); 
    return 0;
}