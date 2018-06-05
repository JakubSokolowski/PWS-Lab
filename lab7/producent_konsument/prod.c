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
  //printf("Producent %d exists!\n", num);
  mqd_t mq = mq_open("/test", O_RDWR | O_NONBLOCK);
  for(int i=0; i < steps; i++) {
      // printf("Producent %d writing step %d\n", num, i);
      ms_type msg;
      msg.pnr = num;
      msg.type = PROD;
      sprintf(msg.text,"Producent %d krok %d",num,i);
      // printf("Text: %s\n", msg.text);
      // Przeslanie komunikatu do kolejki
      mq_send(mq, (char*)&msg, sizeof(ms_type), 0);
      sleep(1);
  }
}
