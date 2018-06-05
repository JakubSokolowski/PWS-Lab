#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

int range_per_process(int max_range, int process_num) {
	return (max_range - (max_range % process_num)) / process_num;
}

// 1st arg - max range, 2nd num of threads
int main(int argc, char * argv[]){
	clock_t begin = clock();
  int pid, status;

	int max_range = atoi(argv[1]);
	int process_num = atoi(argv[2]);

	char from_range[80];
	char to_range[80];

	int current = 0;
	int range = range_per_process(max_range, process_num);


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

	int total_sum = 0;
	for (int i = 0; i < process_num; i++) {
        pid = wait(&status);
        status = WEXITSTATUS(status);
		total_sum += status;
  }
	clock_t end = clock();
	printf("Found %d primes:\n", total_sum);
	printf("Elapsed: %f seconds\n", (double)(end - begin) / CLOCKS_PER_SEC);

	return 0;
}
