#include "def.h"

int main(int argc, char* argv[])
{
    if(argc!=2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) error_handler("socket");
    struct sockaddr_in serv_addr, clnt_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handler("bind");
    
    listen(serv_sock, 5);
    socklen_t clnt_adr_sz;
    int clnt_sock;
    
    /*  stdio로 소켓통신을 한다는 의미가 뭘까?
        buf(uStack) -> stdbuf(uHeap) -> sk_buf (kHeap) 구조로 가겠다는것
        즉, stdbuf로 한번 더 버퍼링(batch)을 해서 성능을 높이기 
        단, full-duplex와 Half-Close를 제대로 구현하려면 파일 스트림을 2개 열어야함 */
   
    char buf[BUF_SZ];
    int len;
    while(1)
    {
        clnt_adr_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
        FILE *fpwrite = fdopen(clnt_sock, "wb");
        FILE *fpread = fdopen(dup(clnt_sock), "rb");
        if(clnt_sock == -1) error_handler("accept");
        /*  fread vs read
        버퍼링의 엄격성에 커다란 차이가 있음.
        read는 인자로 넘긴 BUF_SZ를 못 읽어도 넘기는데
        fread는 BUF_SZ를 못읽으면 읽을때까지 블로킹함. 
        fread의 대안은 fgets; 라인 버퍼링 지원 */

        while(fgets(buf, BUF_SZ, fpread) != NULL)
        {
            // quit에 반응하여 HC
            if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
            {
                shutdown(fileno(fpwrite), SHUT_WR);
                break;
            }
            fputs(buf, fpwrite);
            fflush(fpwrite);
        }
        
        // Last Msg 수신
        fgets(buf, BUF_SZ, fpread);
        puts(buf);
        // shutdown을 해서 WR를 닫아도 fp, fd 정리 의무있음
        fclose(fpread);
        fclose(fpwrite);
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}