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



// 1st arg - max range, 2nd num of threads
int main(int argc, char * argv[]){
  int pid, status;

	int max_range = atoi(argv[1]);
	int process_num = atoi(argv[2]);

	int current = 0;
	int range = range_per_process(max_range, process_num);

  // Create the results.bin file
  int open_fd = open("results.bin",O_TRUNC | O_CREAT, 0666);
  if(open_fd < 0) {
    perror("open");
    exit(1);
  }
  close(open_fd);

  // Create pipe

  int pipe_fd[2];
  pipe(pipe_fd);

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

      // Write to pipe
      close(pipe_fd[0]);
      write(pipe_fd[1], &result, sizeof(result_t));
      close(pipe_fd[1]);
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
  }


  int rd = 0;
  int total_sum = 0;
  close(pipe_fd[1]);
	do {
		result_t result;
		rd = read(pipe_fd[0], &result, sizeof(result_t));
    if(rd == 0)
      break;
    printf(" rd = %d %d, %d, %d \n",rd, result.up_bound, result.low_bound, result.count);
    total_sum += result.count;
    printf("Total sum.. %d\n",result.count);
	} while (rd > 0);

	printf("Found %d primes:\n ", total_sum);
}
