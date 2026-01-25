#include "../def.h"

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        perror("socket() failed");
        exit(1);
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    inet_aton("0.0.0.0", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) == -1){
        perror("bind() error");
        exit(1);
    }

    listen(sock, 5);
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(struct sockaddr_in);
    int clnt_sock, str_len;
    char msg[1024];
    for(int i = 0; i < 5; i++)
    {
        printf("Connection %d\n", i+1);
        clnt_sock = accept(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if(clnt_sock == -1){
            perror("accept() failed");
            exit(1);
        }

        // Ex. msg가 실제로 1024개가 온 경우
        while((str_len = read(clnt_sock, msg, sizeof(msg))) != 0)
        {
            if(str_len == -1){
                perror("read() failed");
                exit(1);
            } 
            write(clnt_sock, msg, str_len);
        }   
    }
    

    return 0;
}