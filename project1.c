/*
Data: Sep.20.2014
Project 1 Matrix Multiple for 7212, Minsheng
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int64_t row,rc,col;
int64_t **A, **B, **C;
FILE *afile, *bfile, *cfile;

struct pos
{
	int64_t i;
	int64_t j;
};

void* calculateIJ(void *param)
{
	struct pos *p = (struct pos*)(param);
	int64_t ii;
	for(ii=0;ii<rc;ii++)
		C[p->i][p->j] += A[p->i][ii]*B[ii][p->j];

	pthread_exit(0);

}

void printout()
{
	int64_t i,j;
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			fprintf(cfile,"%lld",C[i][j]);
			if(j!=col-1)
				fprintf(cfile, ",");
		}
		fprintf(cfile,"\n");
	}
}

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		fprintf(stderr,"usage: a.out intputFile(A) intputFile(B)");
	}

	afile = fopen(argv[1],"r");
	bfile = fopen(argv[2],"r");

	if(afile == NULL)
	{
		fprintf(stderr, "Can't open input file for matrix A!\n");
		return -1;
	}

	if(bfile == NULL)
	{
		fprintf(stderr, "Can't open input file for matrix B!\n");
		return -1;
	}

	//read row, rc and col;
	int64_t a1,a2,a3;
	int64_t b1,b2,b3;
	sscanf(argv[1],"input%lld_%lld_%lld",&a1,&a2,&a3);
	sscanf(argv[2],"input%lld_%lld_%lld",&b1,&b2,&b3);

	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	int64_t i, j, inc, temp;
	char c;
	
	row = a2;
	rc = a3;
	col = b3;
	int64_t lenRow = (int64_t)floor(log10((float)row)) + 1;
	int64_t lenCol = (int64_t)floor(log10((float)col)) + 1;
	char output[20+lenCol+lenRow];

	sprintf(output,"result_%lld_%lld.mat",row,col);

	//needs to finished
	cfile = fopen(output,"wb");

	if(cfile == NULL)
	{
		fprintf(stderr, "Can't open output file for matrix C!\n");
		return -1;
	}

	//initialize A,B,C
	A=(int64_t **)malloc(row*sizeof(int64_t *));

	for(i=0;i<row;i++)
		*(A+i)=(int64_t *)malloc(rc*sizeof(int64_t));

	i = 0; j = 0;
	while(EOF!=(inc=fscanf(afile,"%lld%c",&temp,&c)))
	{
		A[i][j] = temp;
		j++;
		
		if(j/rc>0)
			i++;
		j=j%rc;

		if(i>=row&&j!=0)
		{
			printf("the format of the inputfile(A) is not right.\n");
			return -1;
		}
	}

	if(i<row)
	{
		printf("the format of the inputfile(A) is not right.\n");
		return -1;
	}

	B=(int64_t **)malloc(rc*sizeof(int64_t *));

	for(i=0;i<rc;i++)
		*(B+i)=(int64_t *)malloc(col*sizeof(int64_t));

	i = 0; j = 0;
	while(EOF!=(inc=fscanf(bfile,"%lld%c",&temp,&c)))
	{
		B[i][j] = temp;
		j++;
		
		if(j/col>0)
			i++;
		j=j%col;

		if(i>=rc&&j!=0)
		{
			printf("the format of the inputfile(B) is not right.\n");
			return -1;
		}
	}

	if(i<rc)
	{
		printf("the format of the inputfile(B) is not right.\n");
		return -1;
	}

	C=(int64_t **)malloc(row*sizeof(int64_t *));

	for(i=0;i<row;i++)
		*(C+i)=(int64_t *)malloc(col*sizeof(int64_t)); 

	for(i=0;i<row;i++)
		for(j=0;j<col;j++)
			C[i][j] = 0;

	//calculate C for each element in C generate a thread to calculate the corresponding result
	for(i=0;i<row;i++)
		for(j=0;j<col;j++)
		{
			struct pos *p = (struct pos*) malloc(sizeof(struct pos));
			p->i=i;
			p->j=j;
			pthread_attr_init(&attr);
			pthread_create(&tid,&attr,calculateIJ, (void*)p);
			pthread_join(tid,NULL);
		}

	printout();

	fclose(afile);
	fclose(bfile);
	fclose(cfile);

	return 0;
}