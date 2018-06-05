#include "common.h"
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc,char * argv[]) {
  int num = atoi(argv[1]);
  int steps = atoi(argv[2]);

  mqd_t mq = mq_open("/Queue", O_WRONLY);
  for(int i=0; i < steps; i++) {
    ms_type msg;
    msg.pnr = num;
    msg.type = PROD;
    sprintf(msg.text,"Producent %d krok %d", num, i);
    // Przeslanie komunikatu do kolejki
    mq_send(mq, (char*)&msg, sizeof(ms_type), 10);
    sleep(1);
  }
  mq_close(mq);
}
