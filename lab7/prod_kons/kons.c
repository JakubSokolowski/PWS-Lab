#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include "common.h"

int main(int argc, char* argv[])
{
	if (argc < 3) { printf("Za malo argumentow!K\n"); return 0;}
	int kroki = atoi(argv[2]);
	int id = atoi(argv[1]);

	int fd = shm_open("bufor", O_RDWR, 0666);
	if (fd == -1) { perror("shm_openK"); return 0; }
        printf("fd=: %d\n",fd);

	int i, res, nr;

	bufor_t *wbuf;
	wbuf = (bufor_t*) mmap(0, sizeof(bufor_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (wbuf == NULL) { perror("mmapK"); return 0; }
        printf("mmap\n");

	for (i = 0; i < kroki; i++)
	{
                printf("tail %d\n",wbuf->tail);
		printf("Konsument %d - cnt: %d odebrano %s\n",
			id, wbuf->cnt ,wbuf->buf[wbuf->tail]);
		sem_wait(&(wbuf->full));
		sem_wait(&(wbuf->mutex));
		wbuf-> cnt --;
		wbuf->tail = (wbuf->tail +1) % BSIZE;
		sem_post(&(wbuf->mutex));
		sem_post(&(wbuf->empty));
		sleep(1);
	}

	return 0;
}