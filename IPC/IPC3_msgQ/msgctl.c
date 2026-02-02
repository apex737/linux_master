#include "../def.h"

int main()
{
    key_t key = ftok("keyfile", 'Q');
    // open과 유사하나, 객체는 file이 아닌 메시지큐
    int msgId = msgget(key, IPC_CREAT | 0755); 
    if(msgId == -1) error_handler("msgget");
    puts("Before IPC_RMID");
    system("ipcs -q");

    // 세번째 인자 struct msqid_ds는 뭐하는데 쓰지??
    msgctl(msgId, IPC_RMID, 0);
    puts("After IPC_RMID");
    system("ipcs -q");
    return 0;
}