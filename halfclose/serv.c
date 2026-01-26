#include "../def.h"
#define WR_SZ   30

/*  HALF CLOSE
    TLV 방식으로 APP-프로토콜(패킷 구조체)를 정의해서 헤더(Tag + Length)를 보낸뒤
    Payload를 전달하는 방식이 일반적이다. 그러나, Length 측정 오버헤드가 크리티컬한
    실시간 스트리밍이나 Length 자체가 정의되지 않는 경우 TLV를 쓸 수 없다.
    이러한 문제를 TCP-L4에서 자체적으로 해결하기 위한 송수신 종료 방법이 HC이다.
    
    1. serv.c 파일 전체를 WR_SZ 만큼씩 쪼개서 client에게 보내기
    2. payload 송신 이후 Tx 라인 half-close
    3. 마지막에 Rx 라인으로 클라이언트 응답 받고 종료
*/


int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s\n", argv[0]);
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock==-1) error_handler("sock() error");
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t sock_len = sizeof(serv_addr);
    memset(&serv_addr, 0, sock_len);
    serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr *)&serv_addr, sock_len) == -1)
        error_handler("bind() error");
    
    listen(sock, 5);
    // TCP 통신 Connection Socket
    int clnt_sock = accept(sock, (struct sockaddr *)&clnt_addr, &sock_len);
    char msg[WR_SZ];
    size_t rbyte;
    FILE* fp = fopen("serv.c", "rb"); // 현재 파일을 Byte 단위 읽기 모드로 열기
    if(fp == NULL) error_handler("fopen() error");
    // serv.c 파일을 한번에 WR_SZ(Byte)만큼 읽어서 sock 출력 버퍼에 쓰기
    // fread (Buffer, Chunk_Size, Num, fp)
    while((rbyte = fread(msg, 1, WR_SZ, fp)) > 0)
    {
        // EOF가 뜰 때까지 rbyte만큼 계속 읽는다.
        // 30Byte씩 기본적으로 읽고 남은 자투리도 마저 읽는다.
        write(clnt_sock, msg, rbyte);
    }
    // RX, TX 스트림 라인 중 하나만 제거 가능
    shutdown(clnt_sock, SHUT_WR); // No more Tx
                                  // // TX 버퍼에 남은 데이터 마저 전송
    
    // Payload만 오기 때문에 수동으로 널 추가하기 위한 WR_SZ-1
    if((rbyte = read(clnt_sock, msg, WR_SZ-1)) == -1) error_handler("read() error");
    msg[rbyte] = 0; // memset(msg, 0, sizeof(msg)) 대체
    printf("[Recieved] %s\n", msg);
    
    fclose(fp);
    close(clnt_sock);
    close(sock);
    return 0;
}