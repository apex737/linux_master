#include "../def.h"
#include <sys/select.h>

/*  select 기반 에코+로깅 서버 */
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

    /* Code Start */ 
    
    return 0;
}