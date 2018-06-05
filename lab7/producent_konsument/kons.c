#include "common.h"
#include <stdlib.h>
#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc,char * argv[]) {
    int steps = atoi(argv[1]);
    mqd_t mq = mq_open("/test", O_RDWR | O_NONBLOCK);
    for(int i = 0; i < steps; i++) {
      // Check if the queue is empty
      // printf("Consumer is trying to read something step %d\n", i);
      struct mq_attr attr;
      mq_getattr(mq, &attr);
      // printf("Attr: maxmsg: %ld, size: %ld cur: %ld\n"
      //       , attr.mq_maxmsg
      //       , attr.mq_msgsize
      //       , attr.mq_curmsgs);
      ms_type msg;
      mq_receive(mq, (char*)&msg, sizeof(ms_type), NULL);
      printf("Received: msg type: %d msg pnr: %d, msg text: %s\n"
          , msg.type
          , msg.pnr
          , msg.text);
      sleep(1);
    }
    mq_close(mq);
}
