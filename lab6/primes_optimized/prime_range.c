#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


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


int is_prime(int n) {
    if(n < 2) {
        return 0;
    }
	for(int i = 2; i * i <=n; i++) {
	    if(n % i == 0)
	        return 0;
    }
	return 1;
}

int main(int argc,char * argv[]) {
  int in_fifo_fd, out_fifo_fd;
  if((in_fifo_fd = open("in_fifo", O_RDWR)) < 0) {
    perror("open");
    exit(1);
  }

  if((out_fifo_fd = open("out_fifo", O_RDWR | O_NONBLOCK)) < 0) {
    perror("open");
    exit(1);
  }

  range_t range;
  for(;;) {
    // Read the range from innput queue
    int rd = read(in_fifo_fd, &range, sizeof(range_t));
    // printf("Reading from in_fifo\nrd = %d, Low: %d, Up: %d, Num: %d \n"
    //         , rd
    //         , range.low_bound
    //         , range.up_bound
    //         , range.num);
    // Check if the last range is reached
    if(range.num == -1)
      break;
    // Not last range, compute primes for this range
    int total_primes = 0;
    for(int i = range.low_bound; i < range.up_bound; i++) {
      total_primes += is_prime(i);
    }
    result_t result;
    result.low_bound = range.low_bound;
    result.up_bound = range.up_bound;
    result.count = total_primes;
    // Write the result to out queue
    write(out_fifo_fd, &result, sizeof(result_t));
  }
  close(out_fifo_fd);
  close(in_fifo_fd);
  printf("Range process exiting...");
  exit(0);
}
