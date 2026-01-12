#include <sys/stat.h>
#include "def.h"

/* argc: argument count (n)
   argv: argument vector
   argv[n]; ./exe file_1 file_2 .... file_n-1 */
int main(int argc, char** argv) 
{
  struct stat statbuf; // 파일 정보를 담을 구조체 선언
  if(argc < 2)         // 사용자가 파일명을 입력 안 했을 때 (예: 그냥 ./a.out 만 쳤을 때)
  {
    printf("usage: %s: filename\n", argv[0]); // 올바른 사용법을 알려주고
    return -1;                                // 비정상 종료  
  }
  if(stat(argv[1], &statbuf)) // argv[1](PATH)의 정보를 읽어 statbuf에 채웁니다.
  { 
    printf("stat() fail\n"); // 파일이 없거나 접근 권한이 없으면 실패
    return -1;
  }

  printf("filename %s\n", argv[1]);
  printf("size: %ld\n", statbuf.st_size);
  // !man 7 inode 
  if(S_ISDIR(statbuf.st_mode))
  {
    printf("It is directory\n");
  }
  else if(S_ISREG(statbuf.st_mode))
  {
    printf("It is regular file\n");
  }
  else {
    printf("UNKNOWN\n");
  }
  return 0;
}

