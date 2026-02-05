#include "../def.h"
#include <signal.h>
#include <time.h>

volatile int totalLen = 0;
struct header rxHeader;
struct timespec start, end;
double diff;

// 측정 시작

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }

    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock==-1) error_handler("socket");
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handler("bind");
    
    listen(serv_sock, 5);
    
    int clnt_sock;
    int len;
    char buf[BUF_SZ];

    FILE *fp = fopen("merg.dat", "wb");
    if(fp==NULL) error_handler("fopen");
    
    adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
    
    // 인터럽트 핸들러에서 'H', totalLen 잡아냄 
    if (readn(clnt_sock, &rxHeader, sizeof(rxHeader)) <= 0) 
        error_handler("read header");
    totalLen = rxHeader.size;
    printf("수신 시작: 총 크기 %d bytes\n", totalLen);
    clock_gettime(CLOCK_MONOTONIC, &start);
    while(totalLen > 0)
    {
        struct header pkt;
        // TLV의 목적은 헤더를 파싱해서 이후 길이를 알아내는것 
        if(readn(clnt_sock, &pkt, sizeof(struct header)) <= 0) break;
        if(pkt.tag == 'D')
        {
            if((len = readn(clnt_sock, buf, pkt.size)) <= 0) break;
            fwrite(buf, 1, len, fp);
            totalLen -= len; // 전송받은 순수 데이터만큼 차감
        }
        
    }
    // 측정 종료
    clock_gettime(CLOCK_MONOTONIC, &end);
    // 시간 계산 (초 단위)
    diff = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("소요 시간: %f 초\n", diff);
    close(clnt_sock);
    fclose(fp);
    close(serv_sock);
    return 0;
}


