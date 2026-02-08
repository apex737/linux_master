#include "../def.h"
#include <sys/epoll.h>
#define EP_SIZE 50

/*  epoll 기반 에코+로깅 서버 */


int main(int argc, char* argv[])
{
    if(argc!=2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    int clnt_sock;
    if(serv_sock == -1) error_handler("socket");
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handler("bind");

    if(listen(serv_sock, 5) == -1) error_handler("listen");

    /* Code Start */ 
    /*  1. Wait_List(fd_set)는 리눅스 내부적으로 관리됨 
        2. Wait_List(fd_set)를 Full-Search 하지 않기 때문에 
        Ready List를 위한 공간이 필요함 */
    return 0;
}