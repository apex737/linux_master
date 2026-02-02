#include "../def.h"

int main()
{
    int pd, n;
    char inmsg[100];
    puts("Client ===========");
    if((pd == open("./pipeFile", O_RDONLY)) == -1)
        error_handler("open");
    
    char prnt[] = "From Server: ";
    write(1, prnt, strlen(prnt));
    while((n = read(pd, inmsg, sizeof(inmsg))) > 0){
        write(1, inmsg, n);
    }
    if(n==-1) error_handler("read");
    puts("");
    close(pd);
    return 0;
}