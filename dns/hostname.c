#include "../def.h"
#include <netdb.h> // gethostbyname, gethostbyaddr

// Domain Name => DNS SERVER (Tree 구조) => IP Address

/*  Host Entry 구조체
char *h_name;			// Official name of host.  
char **h_aliases;		// Alias list.  
int h_addrtype;		    // Host address type.   
int h_length;			// Length of address.  
char **h_addr_list;		// List of addresses from name server.  */

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("USAGE: %s <domain name>\n", argv[0]);
        exit(1);
    }
    struct hostent *pHostEntry = gethostbyname(argv[1]);
    printf("Official Name: %s\n", pHostEntry->h_name);
    // 코딩관습; (void)** 는 item이 NULL이면 루프를 빠져나옴 
    for(int i = 0; pHostEntry->h_aliases[i]; i++)  {
        printf("Alias %d: %s\n", i+1, pHostEntry->h_aliases[i]);
    }
    printf("Host Addr Type: %d\n", pHostEntry->h_addrtype);
    printf("Host Length: %d\n", pHostEntry->h_length);
    // 주의: 정의는 char**로 되어있지만 실체는 4byte 주소 (Ex. 1.2.3.4)
    for(int i = 0; pHostEntry->h_addr_list[i]; i++)  {
        printf("IP %d: %s\n", i+1, 
            inet_ntoa(*(struct in_addr*)pHostEntry->h_addr_list[i]));
    }
    return 0;
}