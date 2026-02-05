#include "def.h"

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) error_handler("socket");
    struct sockaddr_in serv_adr;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handler("connect");
    
    // 클라이언트도 dup가 필요할까?
    // 
    FILE* fpread = fdopen(sock, "r");
    FILE* fpwrite = fdopen(sock, "w");
    char buf[BUF_SZ];
    int len;
    while(1)
    {
        printf("Enter Msg: ");
        fgets(buf, BUF_SZ, stdin);
        fputs(buf, fpwrite);
        /*  stdout, stdin은 Line-Buffered
            일반적인 입출력는 Fully-Buffered        
            따라서, fflush-fwrite 계열은 세트처럼 등장한다. */ 
        fflush(fpwrite);
        // EOF or Error
        if(fgets(buf, BUF_SZ, fpread) != NULL){
            printf("[ECHO] %s\n", buf);
        } else break;
            
    }

    fprintf(fpwrite, "[HC RESP] Last Msg...\n");
    // 마지막 메시지
    fflush(fpwrite);
    fclose(fpread);
    fclose(fpwrite);
    close(sock);
    return 0;
}