/*
Data: Oct.15.2014
Project 2 Consumer-Producer problem for 7212, Minsheng
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define BUFFER_SIZE 5 					// buffer size 
#define FACTOR 0.000000001 		// one-billionth
#define TRUE 1

int buffer[BUFFER_SIZE];
sem_t full;
sem_t empty;
pthread_mutex_t mutex;
int count, in, out;

int insertItem(int item);		// inserts item into the buffer
int removeItem(int *item);		// removes an item from the buffer, places it in *item
void *producer(void *prodId);
void *consumer(void *consId);

int main(int argc, char *argv[]) {
	
	int64_t sleepTime, numProducers, numConsumers;
	int64_t i;

	srand(time(NULL));
	
	/* make sure there are four arguments passed in, including the program name */
	if(argc != 4) {
		fprintf(stderr, "\nUsage: <program name> <time allowed for main to sleep> <num of producer threads> <num of consumer threads>\n");
		return -1;
	}
	
	/* error checking */
	if(atoi(argv[1]) <= 0) {
		fprintf(stderr, "Sleep time should be greater than or equal to 0\n");
		return -1;
	}
	
	if(atoi(argv[1]) < 1) {
		fprintf(stderr, "Number of producer threads must be greater than 0\n");
		return -1;
	}
	
	if(atoi(argv[1]) < 1) {
		fprintf(stderr, "Number of consumer threads must be greater than 0\n");
		return -1;
	}
	
	sleepTime = atoi(argv[1]);
	numProducers = atoi(argv[2]);
	numConsumers = atoi(argv[3]);
	count = 0; in = 0; out = 0;
	
	// initialize the buffer 
	for(i=0;i<BUFFER_SIZE;i++)
	{
		buffer[i] = 0;
	}
	
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, 5);	
	pthread_mutex_init(&mutex, NULL);

	/* producer and consumer threads */
	pthread_t producer_threads[numProducers], consumer_threads[numConsumers];
	
	/* create producer threads */	
	for(i=0;i<numProducers;i++) 
	{
		if(pthread_create(&producer_threads[i], NULL, producer, (void *)i) != 0) 
		{
			fprintf(stderr, "Unable to create producer threads");
			exit(1);
		}
	}
	
	/* create consumer threads */
	
	for(i=0;i<numConsumers;i++) {
	
		if(pthread_create(&consumer_threads[i], NULL, consumer, (void *)i) != 0) 
		{
			fprintf(stderr, "Unable to create consumer threads");
			exit(1);
		}
	}
	
	sleep(sleepTime);
	exit(0);
}

int insertItem(int item)
{
	/* insert item into buffer 
	return 0 if successful, otherwise 
	return -1 indicating an error condition */	
	
	if(count < BUFFER_SIZE) {
		buffer[in] = item;
		in = (in + 1) % BUFFER_SIZE;
		count++;
		return 0;
	}
	
	return -1;
}

int removeItem(int *item)
{
	/* remove an object from buffer placing it in item
	return 0 if successful
	return -1 indicating an error condition */
	
	if(count > 0) {
		*item = buffer[out];
		out = (out + 1) % BUFFER_SIZE;
		count--;
		return 0;
	}
	
	return -1;
}

void *producer(void *prodId)
{
	int64_t id = (int64_t)prodId;
	int item, sleepTime;
	
	while(TRUE) {
		/* sleep for x units of time */
		sleepTime = rand() * FACTOR;
		sleep(sleepTime);
		
		/* generate a random number */				
		item = rand();
		
		/* acquire the empty semaphore. Decrease the number of empty buffers */
		sem_wait(&empty);
		
		/* acquire the mutex lock */
		pthread_mutex_lock(&mutex);
		
		if(!insertItem(item))
			printf("producer %lld inserted %d into buffer\n", id, item);
		
		/* release the mutex lock */
		pthread_mutex_unlock(&mutex);
		
		/* release the full semaphore. Increase the number of full buffers */
		sem_post(&full);
	}}

void *consumer(void *consId)
{
	int64_t id = (int64_t)consId;
	int item, sleepTime;
	
	while(TRUE) {
		/* sleep for sleepTime units of time */
		sleepTime = rand() * FACTOR;
		sleep(sleepTime);
		
		/* acquire the full semaphore. Decrease the number of full buffers */
		sem_wait(&full);
		
		/* acquire the mutex lock */
		pthread_mutex_lock(&mutex);
		
		if(!removeItem(&item))
			printf("consumer %lld removed %d from buffer\n", id, item);

		/* release the mutex lock */
		pthread_mutex_unlock(&mutex);
		
		/* release the empty semaphore. Increase the number of empty buffers */
		sem_post(&empty);
	}
}



