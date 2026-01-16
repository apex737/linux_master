#include "def.h"
#include <fcntl.h>
#include <errno.h>

/*  <파일 속성 제어>
    1. Non-blocking 설정
    1) open() Flag O_NONBLOCK
    2) fcntl(fd, cmd, ...) => Practice Point
        - F_GETFL로 Flag 가져와서 
        - F_SETFL로 O_NONBLOCK 추가
    STDIN_FILENO  = 1  
    STDOUT_FILENO = 2 
    STDERR_FILENO = 3
*/

int main()
{
    // fd: Console Input Stream
    int fd = STDIN_FILENO;
    ssize_t ret; 
    char buf[64];
    puts("Blocking Mode Test");
    // Clear Input Buffer
    memset(buf, 0, sizeof(buf));
    // Get Value From Keyboard (Blocking)
    ret = read(fd, buf, sizeof(buf));
    if(ret == -1){ puts("read(failed)"); return -1; }
    else {
        printf("Entered: %s, Size: %ldBytes\n", buf, ret);
    }

    puts("\n"); 
    puts("Non-Blocking Mode Test");
    // Get Prev Flag Options
    int flag = fcntl(fd, F_GETFL); 
    if(flag == -1){puts("Get Flag Failed"); return -1;}
    // Set as Non-Blocking Mode
    if(fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1){puts("Set Flag Failed"); return -1;}    
    // Clear Input Buffer
    memset(buf, 0, sizeof(buf));
    // Get Value From Keyboard (Non-Blocking)
    ret = read(fd, buf, sizeof(buf));
    if(ret == -1){ 
        printf("errno: %d (%s)\n", errno, strerror(errno));
        return -1;
    } else {
        printf("Entered: %s, Size: %ldBytes\n", buf, ret);
    }


    return 0;
}
