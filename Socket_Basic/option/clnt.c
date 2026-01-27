#include "../../def.h"

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char msg[30];
    int str_len;

    if(argc != 2) {
        printf("USAGE: %s <PORT>\n", argv[0]);
        exit(1);
    }

    // 1. socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        error_handler("sock error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 2. connect
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        error_handler("connect() error");
    }

    while(1)
    {
        printf("Enter MSG (Q|q to quit): ");
        
        // sizeof(msg) - 1 까지만 읽고 마지막에 널 문자 붙여줌
        // ovf 발생시 stdin 버퍼에 저장되어 다음 루프에 자동실행됨 (보안이슈) 
        // fgets(msg, sizeof(msg), stdin); 
        if(fgets(msg, sizeof(msg), stdin) == NULL) break;
        if(strcmp(msg, "q\n") == 0 || strcmp(msg, "Q\n") == 0) break;

        if(strchr(msg, '\n') == NULL) 
        {
            int c; 
            // 개행문자나 EOF가 아니면 계속 읽어서 버려라
            while(!((c = getchar()) == '\n' || c == EOF));
            puts("Input Out of range (Truncated)");
            continue; 
        }
      
        
        write(sock, msg, strlen(msg));
        // 최대 30-1 까지 읽고, 마지막에 널문자 삽입
        str_len = read(sock, msg, sizeof(msg)-1);
        if(str_len > 0){
            msg[str_len] = 0;
            printf("Recieved: %s\n", msg);
        }
    }

    close(sock);
    return 0;
}

