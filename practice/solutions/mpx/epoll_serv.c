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
    struct epoll_event readyQ[EP_SIZE];
    struct epoll_event sock_evt;   // 설정값을 넘기기 위한 버퍼
    int ableCnt, len;
    char buf[BUF_SZ];
    // 
    int epfd = epoll_create1(0);    
    if(epfd == -1) error_handler("epoll");
    int fdlog = open("msgfile.dat", 
        O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if(fdlog == -1) error_handler("open");

    // FD_SET, FD_ZERO
    epollin_add(epfd, serv_sock, &sock_evt);
    while(1)
    {
        ableCnt = epoll_wait(epfd, readyQ, EP_SIZE, -1);
        switch (ableCnt)
        {
        case -1:
            puts("epoll_wait failed"); continue;
        case 0:
            puts("epoll timeout"); continue;
        default:
            /*  FD_ISSET: O(N)과 달리 O(M) 
                즉, Ready List만 순회하면 됨 */
            for(int i = 0; i < ableCnt; i++)
            {
                int fd_now = readyQ[i].data.fd;
                if(fd_now == serv_sock)
                {
                    clnt_adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, 
                        (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
                    if(clnt_sock == -1) {
                        puts("accept failed"); continue;
                    }
                    printf("Client %d Connected\n", clnt_sock);
                    epollin_add(epfd, clnt_sock, &sock_evt);
                }
                else
                {
                    len = read(fd_now, buf, BUF_SZ);
                    if(len <= 0){
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd_now, NULL);
                        close(fd_now);
                        printf("Client %d Disconnected\n", fd_now);
                    } else {
                        write(fd_now, buf, len);
                        write(fdlog, buf, len);
                    }
                }
            }
            break;
        }
    }
    close(fdlog);
    close(serv_sock);
    return 0;
}