#include "../def.h"
#define OPSZ        4

int main(int argc, char* argv[])
{
    if(argc!=2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock==-1) error_handler("socket() error");
    struct sockaddr_in serv_addr;
    socklen_t sock_len = sizeof(serv_addr);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(connect(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) 
        error_handler("connect() error");
    
    char msg[64];
    char operator;
    int opint, opnum, res, len;
    while(1)
    {
        printf("Operator (Press Q|q to quit): ");
        scanf(" %c", &operator);
        if(operator == 'q' || operator == 'Q') break;
        memcpy(&msg[0], &operator, 1);

        printf("Opnum (Less than 15): ");
        scanf("%d", &opnum);
        if(opnum >= 15){
            puts("Out of range");
            break;
        } 
        memcpy(&msg[1], &opnum, 4);

        for(int i = 0; i < opnum; i++)
        {
            printf("Operand %d: ", i+1);
            scanf("%d", &opint);
            memcpy(msg+5+OPSZ*i, &opint, OPSZ);
        }
        
        // msg[len] = 0;
        // send
        // write(serv_sock, msg, strlen(msg)); // Is this correct?
        len = 5+OPSZ*opnum;
        write(serv_sock, msg, len);
        read(serv_sock, &res, sizeof(int));
        printf("Result: %d\n", res);
    }
    close(serv_sock);
    return 0;
}