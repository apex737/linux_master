#include "def.h"

// Practice API:  open, close, write, read, fdopen

typedef struct person {
  char name[20];
  int age;
} Person;

int write_info(Person p[2])
{
  int fd;
  ssize_t ret;
  fd = open("file4.dat",  O_WRONLY | O_TRUNC | O_CREAT, 0644);
  for (int i =0; i < 2; i++)
  {
    ret = write(fd, &p[i], sizeof(Person));
    if(ret != sizeof(Person))
      {
        printf("write fail\n");
        close(fd);
        return -1;
      }
    
  }
   
  
  close(fd);
  return 0;
 
}

void read_info()
{
  int fd = open("file4.dat",  O_RDONLY);
  if(fd == -1) return;
  // Read sizeof(Person) starting from p
  // Increase Offset in each Loop
  Person p;
  while(read(fd, &p, sizeof(Person)) > 0)
  {
    printf("name: %s, age: %d\n", p.name, p.age);
  }
  // Convert fd to fp
  // The opposite is fileno (fp->fd)
  FILE* fp = fdopen(fd, "r");
  if(fp == NULL)
  {
    printf("fdopen fail\n");
    close(fd);
    return;
  }
  printf("file position: %ld\n", ftell(fp));
  close(fd);
}


// [문제의 함수] 
void read_info_overwrite(Person p[]) // p는 배열의 시작 주소(예: 100번지)로 고정됨
{
  int fd = open("file4.dat", O_RDONLY);
  if (fd == -1) return;

  while (read(fd, p, sizeof(Person)) > 0) 
  {
    // 1회차: 'Hoon'을 p[0]에 담고 출력
    // 2회차: 'Joon'을 다시 p[0]에 덮어쓰고 출력
    printf("name: %s, age: %d\n", p->name, p->age);
  }
  
  // 루프가 끝나면 p[0]에는 마지막 데이터인 'Joon'만 남아있게 됨
  close(fd);
}


int main()
{
  Person p[] = {
    {"Hoon", 10},
    {"Joon", 20}
  };
  write_info(p);
  read_info();
  printf("p[0] name: %s, age: %d | p[1] name: %s, age: %d\n"
    , p[0].name, p[0].age, p[1].name, p[1].age);

  // Try hexdump file4.dat
  return 0;
}


