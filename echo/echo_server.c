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
    inet_aton(WSL2_IP, &serv_addr.sin_addr);
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
    char msg[1024];

    for(int i = 0; i < 5; i++)
    {
        int clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, 
                            sizeof(struct sockaddr_in));
        if(clnt_sock == -1){
            perror("accept failed");
            exit(1);
        }

        int str_len = read(clnt_sock, msg, sizeof(msg));
        printf("Reciecved: %s\n", msg);
        write(clnt_sock, msg, str_len);

        close(clnt_sock);
    }
    
    close(serv_sock);
    
    return 0;
}