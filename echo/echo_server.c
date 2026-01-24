#include "../def.h"

int main(int argc, char* argv[])
{
    if(argc != 2) {
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    // 1. Create TCP Server Socket
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0); 
    if(serv_sock == -1){
        perror("socket() error");
        exit(1);
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 2. Bind IP & PORT
    if(bind(serv_sock, (struct sockaddr *)&serv_addr, 
        sizeof(struct sockaddr_in)) == -1)
        {
            perror("bind() failed");
            exit(1);
        }
    
    // 3. Listen
    if(listen(serv_sock, 5) == -1){
        perror("listen error");
        exit(1);
    }

    // 4. Accept
    struct sockaddr_in clnt_addr;
    // size_t sockaddr_size = sizeof(struct sockaddr_in);
    socklen_t sockaddr_size = sizeof(struct sockaddr_in);
    char msg[1024];
    int clnt_sock;
    int str_len;
    for(int i = 0; i < 5; i++)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &sockaddr_size);

        if(clnt_sock == -1){
            perror("accept failed");
            exit(1);
        } else {
            printf("Connected Client %d\n", i+1);
        }     
       
        while((str_len = read(clnt_sock, msg, sizeof(msg) - 1)) != 0) {
            msg[str_len] = 0; // printf를 위한 Null 추가 
            printf("Received: %s", msg); 
            write(clnt_sock, msg, str_len); // Null 앞까지 전송
        }
        /*  write가 str_len만큼 보내겠다고 OS에게 알려주고 read가 그만큼만 받으면 되니까
            일견 완벽한 구조처럼 보이지만, TCP는 STREAM 방식이라 항상 가능하지 않다.
            예컨대, 네트워크 상태가 불안정하거나 패킷 사이즈가 너무 길면 Partial Read가 된다.
            따라서, 헤더를 추가하여 Payload Size를 명시하는 추가적인 조치가 필요하다 */
        close(clnt_sock);

    }
    
    close(serv_sock);
    
    return 0;
}





