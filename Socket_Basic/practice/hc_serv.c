#include "../../def.h"

/*  HALF CLOSE 
    Tx 라인만을 끊는 종료 방식
    스트림 데이터의 전송이 끝났음을 APP Protocol 없이 EOF만으로 알려줌 */

int main(int argc, char* argv[])
{
    if(argc!=2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock==-1) error_handler("socket() error");
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t sock_len = sizeof(clnt_addr);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) 
        error_handler("bind() error");
    
    listen(sock, 5);
    int clnt_sock = accept(sock, (struct sockaddr *)&clnt_addr, &sock_len);
    if(clnt_sock == -1) error_handler("accept() error");

    FILE* fp = fopen("op_serv.c", "rb");
    if(fp == NULL) error_handler("fopen() error");
    size_t rbyte;
    char buf[WRSZ];
    // buf, chunk_sz, max_iter_cnt, fp
    while((rbyte = fread(buf, 1, WRSZ, fp)) > 0)
    {
        // file 전송 
        write(clnt_sock, buf, rbyte);
    }
    fclose(fp);
    shutdown(clnt_sock, SHUT_WR);
    // Last ACK MSG(string)
    rbyte = read(clnt_sock, buf, sizeof(buf)-1);
    buf[rbyte] = 0;
    printf("Recieved: %s\n", buf);
    close(clnt_sock); // SHUT_RD
    close(sock);
    return 0;
}