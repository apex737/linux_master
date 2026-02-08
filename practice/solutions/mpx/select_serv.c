#include "../def.h"
#include <sys/select.h>

/*  select 기반 에코+로깅 서버 
    I/O는 표준 입출력 사용할 것 */
int main(int argc, char* argv[])
{
    if(argc!=2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) error_handler("socket");
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz = sizeof(clnt_adr);
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handler("bind");

    if(listen(serv_sock, 5) == -1) error_handler("listen");

    // 1. FD_SET INIT
    fd_set reads, temps;
    int max_fd = serv_sock;
    printf("serv_sock : %d\n", serv_sock);
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    int ableCnt = 0;
    int clnt_sock, len;
    char buf[BUF_SZ];
    FILE *fplog = fopen("msgLog.dat", "wb");
    struct timeval tv;

    while(1)
    {
        temps = reads;
        // max+1? max?
        tv = (struct timeval){5, 0};
        ableCnt = select(max_fd+1, &temps, 0, 0, &tv);
        switch (ableCnt)
        {
            case -1:
                puts("select() error");
                continue;
            case 0:
                puts("timeout restarting...");
                continue;
            
            default:
                for(int i = 0; i < max_fd + 1; i++)
                {
                    if(FD_ISSET(i, &temps)) // reads에 등록된 fd인 경우
                    {
                        if(i == serv_sock)  // connection request
                        {
                            puts("i == serv_sock");
                            clnt_adr_sz = sizeof(clnt_adr);
                            clnt_sock = accept(serv_sock, 
                                (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
                            if(clnt_sock == -1) {
                                puts("accept failed"); continue;
                            }
                            printf("Client %d Connected\n", clnt_sock);
                            /*  half-close를 지원하기 위한 스트림 분할 */ 
                            // reads, max_fd 갱신
                            FD_SET(clnt_sock, &reads);
                            if(max_fd < clnt_sock) max_fd = clnt_sock;
                        }
                        else // service (echo + logging)
                        {
                            len = read(i, buf, BUF_SZ);
                            if(len == 0)
                            {
                                printf("Client %d Disconnected\n", i);
                                close(i);
                                FD_CLR(i, &reads);
                                continue;
                            }
                            write(i, buf, len);
                            fwrite(buf, 1, len, fplog);
                            fflush(fplog);
                        }
                    }
                }

                break;
        }
    }

    fclose(fplog);
    close(serv_sock);
    return 0;
}