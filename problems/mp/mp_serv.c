#include "def.h"

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        return 1;
    }
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) error_handler("socket");
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handler("bind");
    
    listen(serv_sock, 5);
    // 1. pipe로 자식 1은 값을 로깅하고 자식2는 에코 및 전달
    // 2. 부모는 아무것도 안하고 매 accept마다 fork
    pid_t pid;
    int fds[2];
    // IPC를 위한 pipe
    if(pipe(fds) == -1) error_handler("pipe");
    // 로깅을 위한 file
    
    switch (pid = fork())
    {
        case -1: error_handler("fork");
        case 0: 
            // pipe 소비자니까 쓰기는 필요없는듯
            close(fds[1]);
            // 밖에서 fopen하면 fp가 복사될테니까 안될듯 
            FILE* fp = fopen("msgLog.dat", "wb"); 
            char msgBuf[BUF_SZ];
            int msgLen;
            while(1)
            {
                if((msgLen = read(fds[0], msgBuf, BUF_SZ)) > 0)
                {
                    fwrite(msgBuf, 1, msgLen, fp);
                }                
            }
            // dead code?
            fclose(fp);
            return 0;

        default:
            // pipe 생산자니까 읽기 필요없음
            close(fds[0]);
            pid = fork();
            if(pid == 0)
            {
                while(1)
                {
                    adr_sz = sizeof(clnt_adr);
                    int clnt_sock = accept(serv_sock, 
                            (struct sockaddr*)&clnt_adr, &adr_sz);
                    int len;
                    char buf[BUF_SZ];
                    while((len = read(clnt_sock, buf, BUF_SZ)) > 0)
                    {
                        write(clnt_sock, buf, len);
                        write(fds[1], buf, len);
                    }
                    // 매 소켓 접속마다 close 해줘야하나??
                    // close(fds[1]);
                    close(clnt_sock);
                }
               
            }

            // 부모 프로세스
            close(fds[1]);
            return 0;
    }
    
    close(serv_sock);
    return 0;
}