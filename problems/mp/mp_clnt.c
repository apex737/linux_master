#include "def.h"

void produce(int sock, char buf[])
{
    while(1)
    {
        fgets(buf, strlen(buf), stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) break;
        write(sock, buf, strlen(buf));
    }
}

void consume(int sock, char buf[])
{
    int len;
    while(1)
    {
        if((len = read(sock, buf, BUF_SZ-1)) > 0)
        {
            buf[len] = 0;
            printf("ECHO: %s\n", buf);
        }
    }
}


int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        return 1;
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) error_handler("socket");
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(atoi(argv[1]));
    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handler("bind");

    char buf[BUF_SZ];
    pid_t pid = fork();
    if(pid == 0)    // producer (write)
    {
        produce(sock, buf);
    }
    else            // consumer (read)
    {
        consume(sock, buf);
    }

    close(sock);
    return 0;
}