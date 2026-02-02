#include "../def.h"

int main()
{
    int msgId, len;
    msgBuf msg_i;
    key_t key = ftok("keyfile", 'Q');
    // 음수 => 객체 생성에 실패했다!
    if((msgId = msgget(key, 0)) < 0) error_handler("msgget");

    /*  msgtype
        0: 다음 메시지
        양수: msgtype과 같은 type의 메시지
        음수: 크기가 |msgtype| 이하인 메시지  */
    /*  msgflag
        0:
        
    */
    len = msgrcv(msgId, &msg_i, sizeof(msg_i.mtext), 0, 0);
    printf("RECEIVED MSG: %s, LEN: %d\n", msg_i.mtext, len);    

    return 0;
}