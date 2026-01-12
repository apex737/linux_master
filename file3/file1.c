#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

/* File Descriptor

    O_CREAT, 
    O_TRUNC: Delete ALL  Existing Data 
    ESSENTIAL: O_RDONLY, O_WRONLY, or O_RDWR

*/
void append_file()
{
    int fd = open("file", o_wronly | o_append);
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
