#include "../def.h"

typedef struct {
    char operator;
    int opnum;
    int operand[10];
} op_t;

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock==-1) error_handler("socket() error");
    struct sockaddr_in serv_addr;
    socklen_t sock_len = sizeof(serv_addr);
    memset(&serv_addr, 0, sock_len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(connect(sock, (struct sockaddr *)&serv_addr, sock_len) == -1){
        error_handler("connect() error");
    }

    op_t op;
    int res;
    while(1)
    {
        printf("Operator: ");
        scanf(" %c", &op.operator); // white space?
        if(op.operator == 'q' || op.operator  == 'Q') break;
        printf("opnum: ");
        scanf("%d", &op.opnum);
        for(int i = 0; i < op.opnum; i++)
        {
            printf("Operator %d: ", i+1);
            scanf("%d", &op.operand[i]);
        }
        
        write(sock, &op.operator, sizeof(char));
        write(sock, &op.opnum, sizeof(int));
        write(sock, op.operand, sizeof(int)*op.opnum);
        read(sock, &res, sizeof(res));
        printf("Result: %d\n", res);
    }
    close(sock);
    return 0;
}