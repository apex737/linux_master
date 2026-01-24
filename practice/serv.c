#include "../def.h"

int main(int argc, char* argv[])
{
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;

    if(argc != 2)
    {
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handler("bind(error)");
        exit(1);
    }
    
    if(listen(serv_sock, 5) == -1){
        error_handler("listen(error)");
        exit(1);
    }

    int clnt_sock;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, 
        /*(socklen_t *)sizeof(struct sockaddr)*/ &clnt_addr_size);

    char msg[] = "Hello World\n";
    for(int i = 0; i < strlen(msg); i++) {
        write(clnt_sock, &msg[i], 1);
        printf("Sent 1 byte: %c\n", msg[i]);
    }
    // while((wrByte = write(clnt_sock, msg, 1)) == 0)
    // {
    //     if(wrByte == -1){
    //         error_handler("write(error)");
    //         exit(1);
    //     }
    // }
    close(serv_sock);
    return 0;
}