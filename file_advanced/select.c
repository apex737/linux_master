#include "def.h"
#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/time.h>

#if 0

Practice:   
1) 감시할 fd_set 초기화 및 설정
- FD_ZERO		: 초기화
- FD_ISSET		: 상태 확인
- FD_SET, FD_CLR: 상태 Set/Reset

2) Timeout 설정
3) select 

#endif

int main()
{
    int fd = inotify_init();
    int wd1,wd2;
    
    wd1 = inotify_add_watch(fd, ".", IN_CREATE | IN_DELETE);
    wd2 = inotify_add_watch(fd, "../file", IN_CREATE | IN_DELETE);
    if(wd1 == -1){
        puts("wd1 add failed");
        goto err;
    }
    if(wd2 == -1){
        puts("wd2 add failed");
        goto err;
    }

    char buf[1024];
    struct inotify_event* event;
    int ret;
    ssize_t event_size;

    // select, epoll는 실행 이후 fd_set, timeval 구조체를 수정하기 때문에
    // 매 실행마다 초기화해줘야 한다.
    while(1)
    {
        // 1) 감시할 fd_set Init & Register
        // Init fds
        fd_set fds;
        FD_ZERO(&fds);      
        // Register fd, STDIN to fds
        FD_SET(fd, &fds);   
        FD_SET(STDIN_FILENO, &fds);

        // 2) Add timer
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        // 3) Set select API
        int max_fd = fd > STDIN_FILENO ? fd : STDIN_FILENO;
        int sret = select(max_fd + 1, &fds, NULL, NULL, &timeout);
                    
        // 4) Case handle
        if(sret == -1) {
            puts("select() failed\n)");
            return -1;
        } else if (sret == 0) {
            puts("10sec Timeout");
            continue;
        } 

        // On file create or delete        
        if (FD_ISSET(fd, &fds))
        {
            // Fill inotify_event struct into the buffer
            ret = read(fd, buf, sizeof(buf)); 
            char* ptr = buf;
            while(ret > 0)
            {
                event = (struct inotify_event*)ptr;
                event_size = sizeof(struct inotify_event) + event->len;
                if(event->mask & IN_CREATE) {
                    printf("%s created\n", event->name);
                } 
                
                if (event->mask & IN_DELETE) {
                    printf("%s deleted\n", event->name);
                }
                ret -= event_size;
                ptr += event_size;
            }
        }
        
        // On keyboard input
        if (FD_ISSET(STDIN_FILENO, &fds))
        {
            memset(buf, 0, sizeof(buf));
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            printf("you typed: %s\n", buf);
        }
    }
    

    return 0;
err:
    if(fd > 0){
        close(fd);
    }
    return -1;

}
