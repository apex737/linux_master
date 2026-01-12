#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

/* File Descriptor

    O_CREAT, 
    O_TRUNC: Delete ALL  Existing Data 
    ESSENTIAL: O_RDONLY, O_WRONLY, or O_RDWR
*/

typedef struct person {
  char name[20];
  int age;
} Person;

int write_info(Person* p)
{
  int fd;
  fd = open("file",  O_WRONLY | O_APPEND, 0644);
  ssize_t ret = write(fd, &p, sizeof(Person));
  if(ret != sizeof(p->name))
  {
    printf("write fail\n");
    return -1;
  }
   
  return 0;
  close(fd);
}

char buf[20];
void read_info(Person* p)
{
    int fd = open("file",  O_RDONLY);
    ssize_t ret = read(fd, buf, sizeof(buf));

}

void append_file()
{
  int fd = open("file", O_WRONLY | O_APPEND);
  dprintf(fd, "append_file(2)\n"); 
  close(fd);
}

void write_file()
{
    int fd = open("file", O_WRONLY | O_CREAT | O_TRUNC , 0600);
    dprintf(fd, "Hello FIle\n"); 
    close(fd);
}

void read_file()
{
    int fd = open("file", O_RDWR);
    lseek(fd, 0, SEEK_END); 
    // off_t lseek(int fd, off_t offset, int whence);
    dprintf(fd, "append_file()\n"); 
    close(fd);
}


int main()
{
    write_file();
    read_file();
    append_file();
    return 1;
}
