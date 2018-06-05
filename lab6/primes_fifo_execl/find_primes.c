#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


typedef struct {
  int low_bound;
  int up_bound;
  int count;
} result_t;

int range_per_process(int max_range, int process_num) {
	return (max_range - (max_range % process_num)) / process_num;
}

// 1st arg - max range, 2nd num of threads
int main(int argc, char * argv[]){
  int pid, status;

	int max_range = atoi(argv[1]);
	int process_num = atoi(argv[2]);

	char from_range[80];
	char to_range[80];

	int current = 0;
	int range = range_per_process(max_range, process_num);

  // Create FIFO queue
  unlink("primes_fifo");
  if(mkfifo("primes_fifo", 0666) < 0) {
    perror("mkfifo");
  }

  printf("Opening named pipe ...\n");
  int in_fifo_fd;
  if((in_fifo_fd = open("primes_fifo", O_RDWR | O_NONBLOCK)) < 0) {
    perror("open");
    exit(1);
  }

	for(int i = 0; i < process_num; i++) {
		if((pid = fork()) == 0) {
			sprintf(from_range,"%d",current);
			sprintf(to_range,"%d", current + range);
      current += range;
			execl("./prime_range","prime_range",from_range, to_range ,NULL);
			perror("Blad funkcji exec");
		}
    if(i == process_num - 1)
		  range = max_range % process_num;
		current += range;
	}


	// Wait for all the ranges to finish processing
	for (int i = 0; i < process_num; i++) {
        pid = wait(&status);
        status = WEXITSTATUS(status);
  }

  int rd = 0;
  int total_sum = 0;

  printf("Before reading...\n");
  do {
    result_t result;
    rd = read(fifo_fd, &result, sizeof(result_t));
    printf("Reading...\n");
    if(rd <= 0)
      break;
    // printf(" rd = %d, Low: %d, Up: %d,Count: %d \n",rd, result.low_bound, result.up_bound, result.count);
    total_sum += result.count;
    printf("Total sum.. %d\n",total_sum);
  } while (rd > 0);
  printf("Found %d primes:\n ", total_sum);
  exit(0);
}
