#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

int getRandomNumber(int min, int max) {
	double fraction = 1.0 / ((double)(RAND_MAX) + 1.0);
	// Равномерно распределяем рандомное число в нашем диапазоне
	return (int)(rand() * fraction * (max - min + 1) + min);
}

typedef struct data_{
	int first_ind;
	int last_ind;
	int v;
} data_;

typedef struct data_ data;

void* thread_func(void *arg) {
	data *a = (data*)arg;
	int v = 0;
	rand();
	for (int i = a->first_ind; i < a->last_ind; i++) {
		int s = getRandomNumber(1, 51);
		if ((s - 1) / 4 == 51 / 4) {
			a->v++;
		}
	}
	//printf("%d\n", a->v);
}

int main(int argc, char* argv[]) {
	srand((unsigned int)(time(0)));
	int n = 1000;
	int streams = 1;
	if (argc >= 2) {
		n = atoi(argv[1]);
		if (argc == 3) {
			streams = atoi(argv[2]);
		}
		else if (argc > 3) {
			perror("Wrong number of arguments.");
			return 1;
		}
	}
	rand();
	data arg[streams];
	if (streams > n) {
		streams = n;
	}
	//printf("%d\n", streams);
	int step = n / streams;
	pthread_t thread[streams];
	int ind = 0;
	for (int i = 0; i < streams; i++) {
		arg[i].first_ind = 0;
		arg[i].last_ind = step;
		arg[i].v = 0;
		if (i == streams - 1) {
			arg[i].last_ind += n % streams;
		}
		if (pthread_create(&thread[i], NULL, thread_func, &arg[i]) != 0) {
			perror("error");
			return -1;
		} 
	}
	for (int i = 0; i < streams; i++) {
		pthread_join(thread[i], NULL);
	}
	int v = 0;
	for (int i = 0; i < streams; i++) {
		v += arg[i].v;
	}
	printf("%lf \n", (double)v / (double)n);
	return 0;
}
