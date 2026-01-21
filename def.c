#include "def.h"

void error_handler(char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}