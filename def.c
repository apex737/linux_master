#include "def.h"

void error_handler(char* msg)
{
    perror(msg);
    exit(1);
}