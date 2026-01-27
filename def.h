#ifndef DEF_H
#define DEF_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/errno.h>
#define WRSZ    30

void error_handler(char *msg);
ssize_t readn(int fd, void* vptr, size_t n);
typedef struct {
    char operator;
    int opnum;
    int operand[10];
} __attribute__((packed)) op_t;


#endif