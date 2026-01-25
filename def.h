#ifndef DEF_H
#define DEF_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/errno.h>

#define WSL2_IP    "172.29.11.113"
void error_handler(char *msg);

 /*  APP Protocol
    <operand><payload><operator> */
typedef struct _op {
    int cnt;
    int payload[10];
    char operator;
} operate;


#endif