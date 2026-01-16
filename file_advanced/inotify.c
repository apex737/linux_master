#include "def.h"
#include <sys/inotify.h>

#if 0
    struct inotify_event {
        int      wd;       /* Watch descriptor */
        uint32_t mask;     /* Mask describing event */
        uint32_t cookie;   /* Unique cookie associating related
                              events (for rename(2)) */
        uint32_t len;      /* Size of name field */
        char     name[];   /* Optional null-terminated name */
};
#endif

int main()
{
    // 1. Inotify Object 추가(init)
    int fd = inotify_init();
    int wd1, wd2;
    if(fd == -1){
        puts("inotify_init(failed)");
        return -1;
    }
    // 2. Object에 Watch 추가(add_watch)
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
    // 3. 감시 방법은??
    // Read Data that Inotify Object Captured 
    char buf[1024];
    int  ret; 
    struct inotify_event* event;
    // while(1)
    {
        // read(): Blocking API
        ret = read(fd, buf, sizeof(buf));
        if(ret == -1){
            puts("read(failed)");
            return -1;
        }
        // Decode Data with inotify_event struct
        // 읽어낸 이벤트 관련 정보를 buf에 저장
        event = (struct inotify_event*)&buf[0];

        while(ret > 0)
        {
            if(event->mask & IN_CREATE)
            {
                printf("%s created\n", event->name);
            }
            if(event->mask & IN_DELETE)
            {
                printf("%s deleted\n", event->name);
            }
            // Update Pointers
            // [주의] name[]의 크기는 0; sizeof(inotify_event) + len이 실제 크기..
            int event_size = sizeof(struct inotify_event) + event->len;

            event = (struct inotify_event*)( (char*)event + event_size ); 
            // Decrease read() Byte Size as readen
            ret -= event_size;
        }
    }
    
    close(fd);
 //   close(wd1);
 //   close(wd2);

    return 0;

err:
    // 	누구 때문에 점프했는지 모르니까 일단 전부 검사
    if(fd > 0){
        close(fd);
    }
    return -1;

}
