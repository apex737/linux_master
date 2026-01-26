#include "../def.h"
#define RX_SZ       30
/*  1. serv.c 파일을 30Byte 씩 수신
    2. 수신완료하면 응답하고 종료

*/
int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE %s\n", argv[0]);
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock==-1) error_handler("socket() error");
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) 
        error_handler("connect() error");
    
    FILE* fp = fopen("serv_cpy.c", "w");
    if(fp == NULL) error_handler("fopen() error");
    char buf[RX_SZ];
    size_t rxbyte;
    while((rxbyte = read(sock, buf, RX_SZ)) > 0) // HALF_CLOSE 까지 수신
    {
        fwrite(buf, 1, rxbyte, fp); 
    }
    char msg[] = "Recieve Done";
    write(sock, msg, strlen(msg));
    fclose(fp);
    close(sock);
    
    return 0;
}