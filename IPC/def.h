#ifndef DEF_H
#define DEF_H
// #define _GNU_SOURCE     // 모든 확장기능 사용
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/msg.h>
#define BUF_SZ      30
void error_handler(char *msg);
typedef struct msgbuf {
    long mtype;
    char mtext[80];
} msgBuf;

#endif