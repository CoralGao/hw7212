#include <pthread.h>
#include <stdio.h>

#define M 3
#define K 2
#define N 3

int A [M] [K] = { {1,4j, {2,5j, {3,6j }; 
int B [K] [N] = { {8,7,6j, {5,4,3j }; 
int C [M] [N];

struct pos
{
	int i;
	int j;
};

void* calculateIJ(void *param)
{

}

int main()
{
	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	
}