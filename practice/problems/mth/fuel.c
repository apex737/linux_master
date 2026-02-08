#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*  전역변수 fuel를 생산/소비하는 2-스레드 코드 작성
    단, mutex, cond를 사용할 것*/

int fuel = 0;
void* fuel_filling(void* arg) {
    // 매 호출마다 fuel 증가 
    return NULL;
}

void* car(void* arg) {
    // fuel이 40 이상이면 사용가능 
    return NULL;
}

int main()
{

    return 0;
}