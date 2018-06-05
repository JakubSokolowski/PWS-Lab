#include "common.h"
#include <stdlib.h>
#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc,char * argv[]) {
    int steps = atoi(argv[2]);
    mqd_t mq = mq_open("/Queue", O_WRONLY);
    for(int i = 0; i < steps; i++) {
      // Check if the queue is empty
      struct mq_attr attr;
      mq_getattr(mq, &attr);
      if(!attr.mq_curmsgs)
        break;
      ms_type msg;
      mq_receive(mq, (char*)&msg, sizeof(ms_type), NULL);
      printf("%s\n", msg.text);
      sleep(1);
    }
    mq_close(mq);
}
