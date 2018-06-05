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
  int num;
} range_t;

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
  int num_ranges = atoi(argv[3]);

	int current = 0;
	int range = range_per_process(max_range, num_ranges);

  // Create FIFO queue
  unlink("in_fifo");
  if(mkfifo("in_fifo", 0666) < 0) {
    perror("mkfifo");
  }
  unlink("out_fifo");
  if(mkfifo("out_fifo", 0666) < 0) {
    perror("mkfifo");
  }

  printf("Opening named pipes ...\n");
  int in_fifo_fd, out_fifo_fd;
  if((in_fifo_fd = open("in_fifo", O_RDWR | O_NONBLOCK)) < 0) {
    perror("open");
    exit(1);
  }
  if((out_fifo_fd = open("out_fifo", O_RDWR | O_NONBLOCK)) < 0) {
    perror("open");
    exit(1);
  }

  // Spawn child process that will write ranges to in queue
  for(int i = 0; i < process_num; i++) {
    // Spawn proces
    if((pid = fork()) == 0) {
      execl("./prime_range","prime_range",NULL);
    }
  }

  if((pid = fork()) == 0) {
    for(int i = 0; i < num_ranges; i++) {
      // printf("Writing range %d to in_fifo\n", i);
      range_t input_range;
      input_range.low_bound = current;
      input_range.up_bound = current + range;
      input_range.num = i;
      // Write range to fifo
      write(in_fifo_fd, &input_range, sizeof(range_t));
      if(i == num_ranges - 1)
       range = max_range % num_ranges;
      current += range;
   }
   // Write all the ranges that signal the range proceses to finish
   for(int i = 0; i < process_num; i++) {
     printf("Writing terminating range %d to in_fifo\n", i);
     range_t end_range;
     end_range.low_bound = -1;
     end_range.up_bound = -1;
     end_range.num = -1;
     // Write last range to fifo
     write(in_fifo_fd, &end_range, sizeof(range_t));
   }
   exit(0);
  }

	// Wait for all the ranges to finish processing
	for (int i = 0; i < process_num + 1; i++) {
        pid = wait(&status);
        status = WEXITSTATUS(status);
        printf("Proces %d exited with status %d\n", pid, status);
  }

  int rd = 0;
  int total_sum = 0;

  printf("Reading results from queue...\n");
  do {
    result_t result;
    rd = read(out_fifo_fd, &result, sizeof(result_t));
    if(rd <= 0)
      break;
    printf("rd = %d, Low: %d, Up: %d, Count: %d \n",rd, result.low_bound, result.up_bound, result.count);
    total_sum += result.count;
    printf("Total sum.. %d\n",total_sum);
  } while (rd > 0);
  printf("Found %d primes:\n ", total_sum);
  close(out_fifo_fd);
  close(in_fifo_fd);
  exit(0);
}
