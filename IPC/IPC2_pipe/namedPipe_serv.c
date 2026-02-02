#include "../def.h"

/*  Named Pipe for IPC */
int main()
{
    int pd, n;
    char msg[] = "Hello, FIFO";
    puts("Server ===========");
    // 1. Write Block: 수신자가 올 때까지 open에서 Block
    // 2. Write NB: 수신자가 없으면 바로 에러 리턴
    if((pd = open("./pipeFile", O_WRONLY)) == -1)
        error_handler("open");
    
    printf("To Client: %s\n", msg);
    // read할때는 -1로 읽더니, write는 +1로 쓰네?
    // 수신단에서 buf[len] = 0을 하는 수고를 덜기 위함!
    if(n = write(pd, msg, strlen(msg)+1) == -1) 
        error_handler("write");

    close(pd);
    return 0;
}