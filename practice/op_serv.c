#include "../def.h"

typedef struct {
    char operator;
    int opnum;
    int operand[10];
} op_t;

int calc(op_t* p)
{
    int sum;
    switch (p->operator)
    {
    case '+':
        sum = 0;
        for(int i = 0; i < p->opnum; i++) {
            sum += p->operand[i];
        }
        return sum;
    
    case '-':
        sum = p->operand[0];
        for(int i = 1; i < p->opnum; i++) {
            sum -= p->operand[i];
        }
        return sum;
    
    case '*':
        int mul = 1;
        for(int i = 0; i < p->opnum; i++) {
            mul *= p->operand[i];
        }
        return mul;
    
    default:
        return -999;
    }
}


int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s\n", argv[0]);
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) error_handler("sock() error");
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t sock_len = sizeof(serv_addr);
    memset(&serv_addr, 0, sock_len);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(sock, (struct sockaddr *)&serv_addr, sock_len) == -1) 
        error_handler("bind() error");
    
    listen(sock, 5);
    int clnt_sock, res;
    char msg[64];
    op_t op;
    
    while(1)
    {
        clnt_sock = accept(sock, (struct sockaddr *)&clnt_addr, &sock_len);
        puts("Client Connected");
        while(1)
        {
            // Tag -> Len -> Value
            if(readn(clnt_sock, &op.operator, sizeof(char)) <= 0) break;
            if(readn(clnt_sock, &op.opnum, sizeof(int)) <= 0) break;
            if(readn(clnt_sock, op.operand, sizeof(int)*op.opnum) <= 0) break;

            res = calc(&op);
            write(clnt_sock, &res, sizeof(res));
        }
        close(clnt_sock);
        puts("Client Disconnected");
    }
    close(sock);
    return 0;
}

