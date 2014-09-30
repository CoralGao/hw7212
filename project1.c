/*
Data: Sep.20.2014
Project 1 Matrix Multiple for 7212, Minsheng
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int64_t row,rc,col;
int64_t **A, **B, **C;
FILE *afile, *bfile, *cfile;
int64_t num_thrd;

void* multiply(void* portion)
{
  int64_t s = (int64_t)portion;   // retrive the portion info
  int64_t from = (s * row)/num_thrd; 
  int64_t to = ((s+1) * col)/num_thrd;
  int64_t i,j,k;
 
  for (i = from; i < to; i++)
  {  
    for (j = 0; j < row; j++)
    {
      for ( k = 0; k < rc; k++)
 		C[i][j] += A[i][k]*B[k][j];
    }
  }

  return 0;
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
	if(argc != 5)
	{
		fprintf(stderr,"usage: a.out N(threads) intputFile(A) intputFile(B) outputFile(C)");
		return -1;
	}

	afile = fopen(argv[2],"r");
	bfile = fopen(argv[3],"r");
	cfile = fopen(argv[4],"w");

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

	if(cfile == NULL)
	{
		fprintf(stderr, "Can't open output file for matrix C!\n");
	}

	//read row, rc and col;
	int64_t a1,a2,a3;
	int64_t b1,b2,b3;
	sscanf(argv[2],"input%lld_%lld_%lld",&a1,&a2,&a3);
	sscanf(argv[3],"input%lld_%lld_%lld",&b1,&b2,&b3);

	num_thrd = atoi(argv[1]);

	pthread_t tids[num_thrd]; // create N threads to calculate the matrix muliplication.

	int64_t i, j, inc, temp;
	char c;
	
	row = a2;
	rc = a3;
	col = b3;

	// initialize matrix A.
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

	// initialize matrix B.
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

	// initialize matrix C.
	C=(int64_t **)malloc(row*sizeof(int64_t *));

	for(i=0;i<row;i++)
		*(C+i)=(int64_t *)malloc(col*sizeof(int64_t)); 

	for(i=0;i<row;i++)
		for(j=0;j<col;j++)
			C[i][j] = 0;

	for (i = 1; i < num_thrd; i++)
	{
	    // creates each thread working on its own portion of i
	    if (pthread_create (&tids[i], NULL, multiply, (void*)i) != 0 )
	    {
	      perror("Can't create thread");
	      free(tids);
	      exit(-1);
	    }
	}

	// main thread works on portion 0
	multiply(0);
	 
	// main thead waiting for other thread to complete
	for (i = 1; i < num_thrd; i++)
	    pthread_join (tids[i], NULL);

	printout();

	fclose(afile);
	fclose(bfile);
	fclose(cfile);

	return 0;
}