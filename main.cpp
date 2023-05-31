#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <cstdlib>
#include "types_p2.h"
#include "p2_threads.h"
#include "utils.h"

const int NUMTHREADS = 3;
using namespace std;

struct timeval t_global_start;

extern void *input_thread(void *);
extern void *queue_thread(void *);
extern void *room_thread(void *);

int main(int argc, char** argv)
{
	// This is to set the global start time
	gettimeofday(&t_global_start, NULL);


	pthread_t tid[NUMTHREADS];
	int	persons;
	int status = 0;
	int work = 0;
	int	i;

	
	if (argc != 2) 
	{
		fprintf(stderr, "[ERROR] Expected 1 argument, but got (%d) .\n",
			argc - 1);
		fprintf(stderr, "[USAGE] p2_exec <number>\n");
		exit(1);
	}
	if ((persons = atoi(argv[1])) <= 0) 
	{
		fprintf(stderr, "[ERROR] Invalid number of persons: %s\n", argv[1]);
		exit(1);
	}
	if(pthread_create(&tid[0], NULL, input_thread, &persons)) 
	{
		fprintf(stderr, "[ERROR] Error creating input thread\n");
		exit(1);		
	}
	//usleep(MSEC(10));
	if(pthread_create(&tid[1], NULL, queue_thread, NULL)) 
	{
		fprintf(stderr, "[ERROR] Error creating queue thread\n");
		exit(1);		
	}
	//usleep(MSEC(10));
	if(pthread_create(&tid[2], NULL, room_thread, NULL)) 
	{
		fprintf(stderr, "[ERROR] Error creating room thread\n");
		exit(1);		
	}

	/* wait for the threads to finish */
	for (i = 0; i < NUMTHREADS; i++) 
	{
		//printf("Waiting for thread %d to complete\n", i);
		if (pthread_join(tid[i], NULL)) 
		{
			fprintf(stderr, "[ERROR] Error joining thread %d\n", i);	
			exit(1);
		}
	}
	return 0;
}
