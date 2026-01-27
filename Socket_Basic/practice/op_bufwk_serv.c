#include "../../def.h"
#define OPSZ        4

int calc(char operator, int opnum, int operand[])
{
    int sum;
    switch (operator)
    {
    case '+':
        sum = 0;
        for(int i = 0; i < opnum; i++) {
            sum += operand[i];
        }
        return sum;
    
    case '-':
        sum = operand[0];
        for(int i = 1; i < opnum; i++) {
            sum -= operand[i];
        }
        return sum;
    
    case '*':
        int mul = 1;
        for(int i = 0; i < opnum; i++) {
            mul *= operand[i];
        }
        return mul;
    
    default:
        return -999;
    }
}

int main(int argc, char* argv[])
{
    if(argc!=2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock==-1) error_handler("socket() error");
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) 
        error_handler("bind() error");
    
    listen(serv_sock, 5);

    int clnt_sock;
    char operator;
    int opnum;
    int res;
    struct sockaddr_in clnt_addr;
    socklen_t sock_len;
    while(1)
    {   
        // sock_len 버퍼 사이즈는 커널에 의해 실제 수신 크기로 변경되므로 매번 초기화 필요
        sock_len = sizeof(clnt_addr);   
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &sock_len);
        puts("Client Connected");
        while(1)
        {
            // <+><len><operands> parser
            // operator, len을 먼저 read한 이후 operand를 읽는 방식
            if(readn(clnt_sock, &operator, sizeof(char)) <= 0) break;
            if(readn(clnt_sock, &opnum, sizeof(int)) <= 0) break;
            if(opnum >= 15) continue;   // malloc 방어
            int* operand = (int*)malloc(sizeof(int)*opnum);
            for(int i = 0; i < opnum; i++)
            {
                if(readn(clnt_sock, &operand[i], sizeof(int)) <= 0) break;
            }
            res = calc(operator, opnum, operand);
            write(clnt_sock, &res, sizeof(int));
            free(operand);
        }
        puts("Client Disconnected");
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}