#include "common.h"
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc,char * argv[]) {
  int queue_len = atoi(argv[1]);
  int process_num = 10;
  int step_num = 15;
  int pid, status;
  // unsigned int prior = 10;

  mqd_t mq;
  struct mq_attr attr;
  attr.mq_msgsize = sizeof(ms_type);
  attr.mq_maxmsg = queue_len;
  attr.mq_flags = 0;

  mq = mq_open("/Queue", O_RDWR | O_CREAT, 0666, &attr);

  char num[80];
  char steps[80];

  // Spawn producents
  for(int i = 0; i < process_num; i++) {
    if((pid = fork()) == 0) {
      sprintf(num,"%d",i);
      sprintf(steps,"%d", step_num);
      execl("./prod","prod",num,steps,NULL);
    }
  }
  // Spawn consumers
  for(int i = 0; i < process_num; i++) {
    if((pid = fork()) == 0) {
      sprintf(steps,"%d", step_num);
      execl("./kons","kons",steps, NULL);
    }
  }

  // Wait for all the ranges to finish processing
	for (int i = 0; i < process_num * 2; i++) {
        pid = wait(&status);
        status = WEXITSTATUS(status);
  }

  mq_close(mq);
  mq_unlink("/Queue");

}
