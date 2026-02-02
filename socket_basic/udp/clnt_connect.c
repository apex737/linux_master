#include "../def.h"

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s\n", argv[0]);
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock==-1) error_handler("socket() error");
    struct sockaddr_in mb_addr;
    socklen_t sock_len = sizeof(mb_addr);
    memset(&mb_addr, 0, sizeof(mb_addr));
    mb_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    mb_addr.sin_family = AF_INET;
    mb_addr.sin_port = htons(atoi(argv[1]));
    // 클라이언트가 주소 바인드한걸 server는 알 수 없음
    if (connect(sock, (struct sockaddr *)&mb_addr, sizeof(mb_addr)) == -1)
        error_handler("connect() error");

    char msg[128];
    int str_len, total_len;
    while(1)
    {
        printf("Enter MSG: ");
        fgets(msg, sizeof(msg), stdin);
        // if(msg == "q\n" || msg == "Q\n") break;
        if(strcmp(msg, "q\n") == 0 || strcmp(msg, "Q\n") == 0) break;
        total_len = strlen(msg);
        sendto(sock, msg, total_len, 0, (struct sockaddr *)&mb_addr, sock_len);
        str_len = recvfrom(sock, msg, sizeof(msg), 0, 
            (struct sockaddr *)&mb_addr, &sock_len);
        if(str_len == -1){
            error_handler("recvfrom() error");
        } else if (str_len < total_len) {
            error_handler("partial read error");
        } 
        
        printf("Echo: %s\n", msg);

    }
    close(sock);
    return 0;
}