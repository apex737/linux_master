#include "../../def.h"
#define BUF_SIZE 1024
#define PC_IP "127.0.0.1"
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
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(PC_IP);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(connect(sock, (struct sockaddr *)&serv_addr, 
        sizeof(struct sockaddr_in)) == -1)
    {
        perror("connect() failed");
        exit(1);
    }

    char msg[BUF_SIZE];
    int str_len;
    while(1)
    {
        printf("Enter Message: ");
        fgets(msg, BUF_SIZE, stdin);
        if(strcmp(msg, "q\n") == 0 || strcmp(msg, "Q\n") == 0)
        {
            puts("Quit");
            break;
        }
        
        write(sock, msg, strlen(msg));
        str_len = read(sock, msg, BUF_SIZE - 1);
        if (str_len > 0) {
            msg[str_len] = 0; 
            printf("Received MSG: %s\n", msg);
        }   
    }
    close(sock);
    return 0;
}