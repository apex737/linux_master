#include "../../def.h"

int main(int argc, char* argv[])
{
    int serv_sock;  // 연결 요청 Await
    int clnt_sock;  // 클라이언트 전용 수신
    struct sockaddr_in serv_addr; // server 옵션 설정
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        error_handler("socket error");
    }

    /* -------------------------- Option 1 ------------------------ */
    int sndbuf, rcvbuf;
    socklen_t sndsz = sizeof(sndbuf);
    getsockopt(serv_sock, SOL_SOCKET, SO_SNDBUF, (void*)&sndbuf, &sndsz);
    getsockopt(serv_sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcvbuf, &sndsz);
    printf("rxbuf size: %d\n", rcvbuf);
    printf("txbuf size: %d\n", sndbuf);
    /* -------------------------- Option 2 ------------------------ */
    int option;
    socklen_t optlen = sizeof(option);
    option = true;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);



    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){
        error_handler("bind() error");
    }

    if(listen(serv_sock, 5) == -1){
        error_handler("listen() error");
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1){
        error_handler("clnt_sock() error");
    }
    
    int str_len;
    char buf[30];
    while((str_len = read(clnt_sock, buf, sizeof(buf))) != 0)
    {
        write(clnt_sock, buf, str_len);
        write(STDOUT_FILENO, buf, str_len);
    }
    close(clnt_sock);
    close(serv_sock);
    return 0;
}
