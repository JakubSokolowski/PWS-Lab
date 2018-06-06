#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include "common.h"

int main(int argc, char* argv[])
{
	if (argc < 3) { printf("Za malo argumentow!P\n"); return 0;}
	int id = atoi(argv[1]);
	int kroki = atoi(argv[2]);

	int fd = shm_open("bufor", O_RDWR, 0666);
	if (fd == -1) { perror("shm_openP"); return 0; }

	int i, res;
	bufor_t *wbuf;
	wbuf = (bufor_t*) mmap(0, sizeof(bufor_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (wbuf == NULL) { perror("mmapP"); return 0; }

	for (i = 0; i < kroki; i++)
	{
		printf("Producent %d - cnt:%d head: %d tail: %d\n",
			id, wbuf-> cnt,wbuf->head,wbuf->tail);
		sem_wait(&(wbuf->empty));
		sem_wait(&(wbuf->mutex));

		sprintf(wbuf->buf[wbuf->head],"Komunikat %d",i);
		wbuf-> cnt++;
		wbuf->head = (wbuf->head +1) % BSIZE;
		sem_post(&(wbuf->mutex));
		sem_post(&(wbuf->full));
		sleep(1);
	}

	return 0;
}