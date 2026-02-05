#include "../def.h"
#include <netinet/tcp.h>    // NO_DELAY
#include <sys/uio.h>
#include <sys/stat.h>


int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) error_handler("socket");
    struct sockaddr_in serv_adr;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_adr.sin_port = htons(atoi(argv[1]));

    // File Open
    FILE* fpsrc[2] = {
        fopen(FILE1, "rb"), 
        fopen(FILE2, "rb")
    };

    if(fpsrc[0] == NULL || fpsrc[1] == NULL) error_handler("fopen");
    
    struct stat sb;
    size_t fileSize = 0;
    int i;
    for(i = 0; i < 2; i++)
    {
        if(fstat(fileno(fpsrc[i]), &sb) == -1) error_handler("fstat");
        fileSize += sb.st_size;
    }

    // nagle off
    int option = 1; // TRUE
    socklen_t optsize = sizeof(option);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &option, optsize);
    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handler("connect");
    
    // 먼저 헤더 보내기
    struct header head = {(int)fileSize, 'H'};
    send(sock, &head, sizeof(head), 0);

    // payload 전송 
    int len;
    char buf[BUF_SZ];
    struct header datHeader;
    struct iovec iov[2];
    for(i = 0; i < 2; i++)
    {
        // BUF_SZ 만큼씩 반복적으로 buf에 퍼담는다
        while((len = fread(buf, 1, BUF_SZ, fpsrc[i])) > 0)
        {
            // 헤더 먼저 만들고
            datHeader.size = len;
            datHeader.tag = 'D';
            // buf(payload)와 뭉쳐서 한번에 전달
            iov[0] = (struct iovec){&datHeader, sizeof(datHeader)};
            iov[1] = (struct iovec){&buf[0], len};
            writev(sock, iov, 2);
        }
    }
    
    for(i = 0; i < 2; i++){
        fclose(fpsrc[i]);
    }
    
    return 0;
}