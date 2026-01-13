#include "def.h"
#include <sys/mman.h>
#include <sys/stat.h>

typedef struct {
    char name[20];
    int  age;
} Person;


// static int write_info(Person p[])
// The array length cannot be transmitted by itself
static int write_info(Person p[], int cnt)
{  //  int open(const char *pathname, int flags, ...
   //                /* mode_t mode */ );
   int fd = open("persons", O_CREAT | O_WRONLY | O_TRUNC, 0644);
   if(fd==-1)
   {
       printf("open() failed\n");
       close(fd);
       return -1;
   }
   
   // mmap can only understand the ByteStream (No dprintf!)
   if(write(fd, p, sizeof(Person)*cnt)==-1) // write p[i] to fd
    {
        printf("write() failed\n");
        close(fd);
        return -1;
    }
   close(fd);
   return 0;
}

// read file using mmap API
static int read_info_mmap(void)
{
    // 1. mmap target : file opened
    int fd = open("persons", O_RDONLY);
    if(fd==-1)
    {
       printf("open() failed\n");
       close(fd);
       return -1;
    }
    // get stat buffer to use st_size
    struct stat st;
    if(fstat(fd, &st) == -1)
    {
        printf("fstat() failed\n");
        close(fd);
        return -1;
    }
    //  void *mmap(void addr[.length], size_t length, int prot, 
            // int flags, int fd, off_t offset);
    // Define the pointer stride 
    Person* p = (Person*) mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED)
    {
        printf("mmap failed\n");
        close(fd);
        return -1;
    }

    // read and print
    for(int i = 0; i < (int)(st.st_size / sizeof(Person)); i++)
    {
        printf("name: %s, age: %d\n",
                p[i].name, p[i].age);
    }

    munmap(p, st.st_size);
    return 0;
}

int main()
{
    Person persons[] = {
        {"Hoon", 10},
        {"Joon", 20},
        {"Koon", 30}
    };
    write_info(persons, 3);
    read_info_mmap();
    return 0;
}
