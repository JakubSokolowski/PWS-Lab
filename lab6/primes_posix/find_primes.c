#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <mqueue.h>


typedef struct {
  int low_bound;
  int up_bound;
  int count;
} result_t;

int range_per_process(int max_range, int process_num) {
	return (max_range - (max_range % process_num)) / process_num;
}
int is_prime(int n) {
    if(n < 2) {
        return 0;
    }
	for(int i = 2; i * i <=n; i++) {
	    if(n % i == 0)
	        return 0;
    }
    printf("Found prime: %d\n", n);
	return 1;
}
int find_prime_in_range(int lower_bound, int upper_bound) {
  int total_primes = 0;
	for(int i = lower_bound; i < upper_bound; i++) {
		total_primes += is_prime(i);
	}
  return total_primes;
}
//Opening named pipe
// 1st arg - max range, 2nd num of threads
int main(int argc, char * argv[]){
  int pid, status;

	int max_range = atoi(argv[1]);
	int process_num = atoi(argv[2]);

	int current = 0;
	int range = range_per_process(max_range, process_num);

  // Create POSIX queue
  unsigned int prior;
  mqd_t mq;
  mq_attr attr;
  attr.mq_msgsize = sizeof(result_t);
  attr.mq_maxmsg = process_num;
  attr.mq_flags = 0;

  mq = mq_open("/Queue", O_RDWR | O_CREAT, 0666, &attr);


	for(int i = 0; i < process_num; i++) {
    // Child Process
		if((pid = fork()) == 0) {
      // Compute the range
      int lower = current;
      int upper = current + range;
      int count = find_prime_in_range(lower, upper);

      // Crete the result struct
      result_t result;
      result.low_bound = lower;
      result.up_bound = upper;
      result.count = count;

      mqd_t queue = mq_open("/Queue", O_WRONLY);
      mq_send(queue, (char*) &result, sizeof(result), 0);
      mq_close(queue);
      exit(0);
		}
    if(i == process_num - 1)
		  range = max_range % process_num;
		current += range;
	}

	// Wait for all the ranges to finish processing
	for (int i = 0; i < process_num; i++) {
    pid = wait(&status);
    status = WEXITSTATUS(status);
    printf("Process %d finished with status:  %d\n", pid,status);
  }


  int rd = 0;
  int total_sum = 0;

  printf("Before reading...\n");
  for(;;) {

  }
	do {
		result_t result;
    mq_receive(mq, (char*) &result, sizeof(result_t), NULL);
    printf("Reading...\n");
    if(rd <= 0)
      break;
    printf(" rd = %d, Low: %d, Up: %d,Count: %d \n",rd, result.low_bound, result.up_bound, result.count);
    total_sum += result.count;
    printf("Total sum.. %d\n",total_sum);
	} while (rd > 0);
	printf("Found %d primes:\n ", total_sum);
  mq_close(mq);
  mq_unlink("/Queue");
  exit(0);
}
