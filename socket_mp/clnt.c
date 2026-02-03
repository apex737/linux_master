#include "def.h"

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) error_handler("socket");
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handler("connect");

    char buf[64];
    int len;
    while(1)
    {
        printf("ENTER MSG (press Q|q to quit): ");
        if(fgets(buf, sizeof(buf), stdin) == NULL){
            while(getchar() != '\n');
        }
        if(strcmp(buf, "q\n") == 0 || strcmp(buf, "Q\n") == 0) break;
        write(sock, buf, strlen(buf));  // "hi\n\0"
        if((len = read(sock, buf, sizeof(buf)-1)) > 0)
        {
            buf[len] = 0;
            printf("ECHO: %s\n", buf);
        }
        
    }

    close(sock);
    return 0;
}