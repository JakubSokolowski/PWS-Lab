#include <stdlib.h>
#include <stdio.h>
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
	exit(total_primes);
}
