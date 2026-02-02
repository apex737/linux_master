#include "../def.h"

int main()
{
    int pd, n;
    char inmsg[100];
    puts("Client ===========");
    //  1. Read Block: 송신자가 쓸 때까지 open에서 Block
    /*  2. Read NB: 송신자가 없으면 바로 에러 리턴
        중요*: 서버가 Block이면 풀리는 타이밍에 값을 바로 읽을 수가 없어서 
        EAGAIN을 띄우므로, 이 경우를 위한 예외처리가 필요함  */ 
    
    if((pd = open("./pipeFile", O_RDONLY | O_NONBLOCK)) == -1)
        error_handler("open");
    
    char prnt[] = "From Server: ";
    write(1, prnt, strlen(prnt));
    // 클라이언트 코드 수정 제안
    while(1) {
        n = read(pd, inmsg, sizeof(inmsg));
        if (n > 0) { // 데이터 읽기 성공
            write(1, inmsg, n);
            break;
        } 
        if (n == -1 && errno == EAGAIN) { // 데이터가 아직 안 왔을 뿐임
            continue; // 다시 시도 (또는 다른 작업 수행)
        }
        if (n == 0) break; // 상대방이 닫음
        error_handler("read"); // 진짜 에러
    }
    puts("");
    close(pd);
    return 0;
}