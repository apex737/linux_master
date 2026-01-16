#include "def.h"
#include <sys/inotify.h>
#include <sys/epoll.h>

int main()
{
    int fd = inotify_init();
    if(fd == -1){
        puts("inotify_init() failed");
        return -1;
    }

    inotify_add_watch(fd, ".", IN_CREATE | IN_DELETE);
    inotify_add_watch(fd, "../file", IN_CREATE | IN_DELETE);

    // 1. Epoll Object Create
    int epfd = epoll_create1(0);
    if(fd == -1){
        puts("epoll_create() failed");
        return -1;
    }

    // 2. Epoll Setting
    // int epoll_ctl(int epfd, int op, int fd,
    //          struct epoll_event *_Nullable event);
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    // How to set event field?
    event.events = EPOLLIN;
    event.data.fd = fd;
    // How to set op field?
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    
    memset(&event, 0, sizeof(event));
    event.events = EPOLLIN;         // read mode
    event.data.fd = STDIN_FILENO;   
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    
    char buf[1024];
    struct inotify_event* inot_event;
    char* ptr;
    int ret;
    int event_size;
    int er;

    while(1)
    {
        // Why the MaxEvent is 1?
        er = epoll_wait(epfd, &event, 1, 10000); // block for maximum 10sec
        switch(er)
        {
            case -1: puts("epoll_wait(failed)"); return -1;
            case  0: puts("10SEC OVER TIMEOUT!!!"); continue;
        }

        if(event.data.fd == fd)
        {
            ret = read(fd, buf, sizeof(buf));
            ptr = buf;
            while(ret > 0)
            {
                inot_event = (struct inotify_event*)ptr;
                event_size = (sizeof(struct inotify_event)+inot_event->len);
                if(inot_event->mask & IN_CREATE){
                    printf("%s created\n", inot_event->name);    
                }
                if(inot_event->mask & IN_DELETE){
                    printf("%s deleted\n", inot_event->name);    
                }
                ret -= event_size;
                ptr += event_size;
            }
        }

        if(event.data.fd == STDIN_FILENO){
            memset(buf, 0, sizeof(buf));
            read(STDIN_FILENO, buf, sizeof(buf));
            printf("you typed %s\n", buf);
        }
    }
    
    return 0;
}
