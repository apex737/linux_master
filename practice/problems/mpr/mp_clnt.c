#include "def.h"
#include <signal.h>
#include <wait.h>

/* IO 스트림 분리해서 클라이언트 코드 작성 */
void produce(int fd, char buf[]);
void consume(int fd, char buf[]);

int main(int argc, char* argv[])
{
    
}
