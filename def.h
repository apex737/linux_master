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

void error_handler(char *msg);
ssize_t readn(int fd, void* vptr, size_t n);
typedef struct _op {
    int cnt;
    int payload[10];    // 개선 필요
    char operator;
} operate;


#endif