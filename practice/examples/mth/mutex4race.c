#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int x = 0;
/*  독자적 실행흐름을 위해 레지스터, 스택은 고유한 값은 가지나
    메모리는 공유하기 때문에 나타남
    연산의 중간결과를 공유하지 않기 때문에 한쪽의 기여도가 무시되는
    현상도 발생가능 */
pthread_mutex_t mutex; 

void* routine(void* arg)
{
    // Race Condition
    // 적은 연산은 컨텍스트 체인지의 경우를 방지하기 때문에
    // 타임슬라이스 인터럽트 전에 하나의 스레드에서 연산이 완료됨
    for(int i =0; i< 100*100*100;i++){
        pthread_mutex_lock(&mutex);
        x++; // 임계구역 (not atomic)
        pthread_mutex_unlock(&mutex);
    }
        
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_t t1,t2;
    pthread_create(&t1, NULL, routine, NULL);
    pthread_create(&t2, NULL, routine, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);
    printf("Value of x: %d\n", x);
}