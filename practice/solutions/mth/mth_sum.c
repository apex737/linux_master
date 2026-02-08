#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define PSIZE          12
int prime[PSIZE] = {2,3,5,7,11,13,17,19,23,29,31,37};

/* 스레드 4개를 써서 병렬적으로 배열의 총합 구하기 */

void* routine(void *arg)
{
    int start = *(int *)arg;
    int sum = 0;
    for(int i = start; i < start+3; i++)
    {
        sum += prime[i];
    }
    *(int *)arg = sum;
    return (void *)arg;
}

int main()
{
    pthread_t th[4];
    int start[4];
    for(int i = 0; i < 4; i++)
    {
        start[i] = i*3;
        pthread_create(&th[i], NULL, routine, &start[i]);
    }
    
    int *ret;
    int totalSum = 0;
    for(int i = 0; i < 4; i++)
    {
        pthread_join(th[i], (void**)&ret);
        totalSum += *ret;
    }
    printf("totalSum: %d\n", totalSum);
    return 0;
}