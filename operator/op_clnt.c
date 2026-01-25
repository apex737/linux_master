#include "../def.h"

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: <port>");
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) error_handler("socket() error");
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    operate op;
    char c;
    while(1)
    {
        printf("Do you want to continue? (y/n) ");
        scanf(" %c", &c);
        if(c == 'n' || c == 'N') break;
        printf("Enter opnum: ");
        scanf("%d", &op.cnt);
        for(int i = 0; i < op.cnt; i++)
        {
            printf("Enter operand %d: ", i);
            scanf("%d", &op.payload[i]);
        }
        
        printf("Enter operator: ");
        scanf(" %c", &op.operator);

        write(sock, &op.operator, sizeof(char));
        write(sock, &op.cnt, sizeof(int));
        write(sock, op.payload, sizeof(int) * op.cnt);
        int result;
        read(sock, &result, sizeof(result));
        printf("Result: %d\n", result);
        
    }
    close(sock);
    return 0;
}