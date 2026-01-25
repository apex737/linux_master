#include "../def.h"

void makeSock(int* pSock, char* argv1)
{
    *pSock = socket(PF_INET, SOCK_STREAM, 0);
    if(*pSock == -1){
        perror("socket() failed");
        exit(1);
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(atoi(argv1));    // 값만 보내면 됨

    if(connect(*pSock, (struct sockaddr *)&serv_addr, 
        sizeof(struct sockaddr_in)) == -1)
    {
        perror("connect() failed");
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    int sock;
    char msg[1024];
    int str_len;
    while(1)
    {
        printf("Enter MSG: ");
        fgets(msg, sizeof(msg), stdin); // fgets는 널 문자까지 붙여준다
        makeSock(&sock, argv[1]);
        write(sock, msg, strlen(msg));
        str_len = read(sock, msg, sizeof(msg)-1);
        if(str_len == -1){
            perror("read() failed");
            exit(1);
        } else {
            msg[str_len] = 0;
            printf("Echo %s\n", msg);
        }
       
        close(sock);
    }
    return 0;
}