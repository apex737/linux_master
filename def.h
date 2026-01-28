#ifndef DEF_H
#define DEF_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/times.h>
#define WRSZ    30

void error_handler(char *msg);
ssize_t readn(int fd, void* vptr, size_t n);
typedef struct {
    char operator;
    int opnum;
    int operand[10];
} __attribute__((packed)) op_t;


#endif