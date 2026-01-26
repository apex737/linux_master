#include "../def.h"

/*  리처드 스티븐슨의 readn 함수와 App-Protocol을 사용하여 경계가 존재하지 않는
    TCP의 특성으로 인해 partial read가 일어나는 상황을 방지한다. */

int calc(int cnt, int payload[], char operator)
{
    int sum, mul, i;
    switch (operator)
    {
    case '+':
        puts("Summation Start");
        sum = 0;
        for(i = 0; i < cnt; i++) sum += payload[i];
        return sum;
    case '*':
        puts("Multiplication Start");
        mul = 1;
        for(i = 0; i < cnt; i++) mul *= payload[i];
        return mul;
    case '-':
        puts("Subtraction Start");
        sum = payload[0];
        for(i = 1; i < cnt; i++) sum -= payload[i];
        return sum;    
    default:
        return -999;
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: <port>");
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) error_handler("socket() error");
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t slnt_addr_sz = sizeof(clnt_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){
        error_handler("bind() error");
    }

    listen(sock, 5);
    int clnt_sock;
    operate op;
    
    // 서버 코드 수정
    while(1)
    {
        clnt_sock = accept(sock, (struct sockaddr *)&clnt_addr, &slnt_addr_sz);
        if(clnt_sock == -1) error_handler("accept() failed");
        puts("Client Connected");

        // 이 클라이언트가 나갈 때까지 반복 서비스
        while(1)
        {
            // 1. 헤더 수신 (Tag + Len)
            if (readn(clnt_sock, &op.operator, sizeof(char)) <= 0) break;
            if (readn(clnt_sock, &op.cnt, sizeof(int)) <= 0) break;
            // 2. Payload (Val) 수신
            if (readn(clnt_sock, op.payload, sizeof(int) * op.cnt) <= 0) break;
            
            // 2. 계산 및 전송
            int result = calc(op.cnt, op.payload, op.operator);
            write(clnt_sock, &result, sizeof(result));
        }
        close(clnt_sock); // 한 클라이언트 서비스 종료 후 닫기
        puts("Client disconnected...");
    }
    

    return 0;
}