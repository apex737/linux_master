#include "../def.h"


int main()
{
    key_t key;
    int msgId;
    msgBuf msg_o;
    // 몇동 몇호
    key = ftok("keyfile", 'Q');
    msgId = msgget(key, IPC_CREAT | 0644); // ??이게  왜가능하지?
    if(msgId == -1) error_handler("msgget");

    msg_o.mtype = 1; // ?? 걍 내맘대로 함?
    strcpy(msg_o.mtext, "MSGQ TEST");
    // Non-Block; flag가 0이면 WAIT
    if(msgsnd(msgId, (void*)&msg_o, sizeof(msg_o.mtext), IPC_NOWAIT) == -1)
        error_handler("msgsnd");
    
    return 0;   
}