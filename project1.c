#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define M 3
#define K 2
#define N 3

int A [M] [K] = { {1,4}, {2,5}, {3,6} }; 
int B [K] [N] = { {8,7,6}, {5,4,3} }; 
int C [M] [N];

struct pos
{
	int i;
	int j;
};

void* calculateIJ(void *param)
{
	struct pos *p = (struct pos*)(param);
	int ii;
	for(ii=0;ii<K;ii++)
		C[p->i][p->j] += A[p->i][ii]*B[ii][p->j];

	pthread_exit(0);

}

void output()
{
	int i,j;
	for(i=0;i<M;i++)
	{
		for(j=0;j<N;j++)
		{
			printf("%d ",C[i][j]);

		}
		printf("\n");
	}
}

int main()
{
	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	int i, j;
	for(i=0;i<M;i++)
		for(j=0;j<N;j++)
		{
			struct pos *p = (struct pos*) malloc(sizeof(struct pos));
			p->i=i;
			p->j=j;
			
			pthread_attr_init(&attr);
			pthread_create(&tid,&attr,calculateIJ, (void*)p);
			pthread_join(tid,NULL);
		}
	output();
	return 0;

}