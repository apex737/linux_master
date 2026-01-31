#include "../def.h"

int main()
{
    // cmd를 pipe_i로 입력
    FILE *fp = popen("date", "r");
    if(fp == NULL) error_handler("popen");
    char buf[256];
    if(fgets(buf, sizeof(buf), fp) == NULL)
        error_handler("No data from pipe");
    
    printf("line: %s\n", buf);
    pclose(fp);
    return 0;
}