#include "../../def.h"

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
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) 
        error_handler("connect() error");
    
    FILE* fp = fopen("op_serv_cpy.c", "wb");
    if(fp == NULL) error_handler("fopen() error");
    char buf[WRSZ];
    size_t rbyte;
    while((rbyte = read(sock, buf, sizeof(buf))) > 0)
    {
        fwrite(buf, 1, rbyte, fp);
    }
    fclose(fp);
    snprintf(buf, sizeof(buf), "File Copy Done\n");
    write(sock, buf, strlen(buf));
    close(sock);
    return 0;
}