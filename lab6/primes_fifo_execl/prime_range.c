#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


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
    printf("Found prime: %d\n", n);
	return 1;
}

int main(int argc,char * argv[]) {
	int from = atoi(argv[1]); int to = atoi(argv[2]);
	int total_primes = 0;
  printf("Process range: %d %d\n", from, to);
	for(int i = from; i < to; i++) {
		total_primes += is_prime(i);
	}

  result_t result;
  result.low_bound = from;
  result.up_bound = to;
  result.count = total_primes;

  int fifo_fd;

  if((fifo_fd = open("primes_fifo", O_RDWR)) < 0) {
    perror("open");
    exit(1);
  }
  write(fifo_fd, &result, sizeof(result_t));
  close(fifo_fd);
  exit(0);
}
