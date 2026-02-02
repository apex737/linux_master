#include "../def.h"


int main(int argc, char* argv[])
{
    int serv_sock;  // 연결 요청 Await
    int clnt_sock;  // 클라이언트 전용 수신
    struct sockaddr_in serv_addr; // server 옵션 설정
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    
    char msg[] = "Hello World!\n";
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }

    // 1. socket (전화기(fd) 생성)
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        error_handler("socket error");
    }

    // 2. serv_addr 구조체로 서버 주소 옵션 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // 포트명만 일치하면 어떤 IP든 다 받겠음
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 3. bind (전화기(fd)에 옵션 할당)
    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){
        error_handler("bind() error");
    }

    // 4. listen (전화기 연결; 최대 5개까지 큐로 받겠음)
    if(listen(serv_sock, 5) == -1){
        error_handler("listen() error");
    }

    clnt_addr_size = sizeof(clnt_addr);
    // 5. accept (전화기 수신 허용)
    clnt_sock = accept(
        serv_sock, 
        (struct sockaddr *)&clnt_addr, 
        &clnt_addr_size
    );

    if(clnt_sock == -1){
        error_handler("clnt_sock() error");
    }

    // client_sock은 메시지 큐(우편함) 열쇠
    write(clnt_sock, msg, sizeof(msg));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

