#ifndef DEF_H
#define DEF_H
#define _GNU_SOURCE     // 모든 확장기능 사용
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#define WRSZ    32
#define BUF_SZ  WRSZ
#define FILE1   "oob_recv.c"
#define FILE2   "oob_send.c"

// 5byte로 맞추기
#pragma pack(push, 1) // 1byte 단위 정렬
struct header {
    int size;
    char tag;
};
#pragma pack(pop)

void error_handler(char *msg);
ssize_t readn(int fd, void* vptr, size_t n);
typedef struct {
    char operator;
    int opnum;
    int operand[10];
} __attribute__((packed)) op_t;


#endif