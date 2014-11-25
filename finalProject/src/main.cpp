/*
Author: Minsheng Zhang
		Shanshan Gao
*/

#include "parserInput.hpp"
#include "tileToSetCover.hpp"
#include "dancinglink.hpp"
#include <iostream>
#include <vector>
#include "def.h"
//#include <mpi.h>
//#define MAX_PROC_NUMBER 100

using namespace std;
int dataset[10000];
int metadata[MAX_PROC_NUMBER * 2];
int totalsubset = 0;
int totalSolutions = 0;

DL *dl = NULL;

int scatterMetaData(int *data, int *metadata, int metadatasize, int P, int p);

int 
scatterMetaData(int *data,
				int *metadata,
				int metadatasize,
				int P,
				int p)
{	
	int	pN = P,	
		idx = 0,
		tag = 101,
		rcvMetadatasize = metadatasize,
		sndMetadatasize = metadatasize,
		datasize,
		newN;

	bool 	isMaster = (p == 0);

	MPI_Status	status;

	if(!isMaster)
	{
		int k = (int) ceil(pN*1.0/2);

		while(p != k)
		{
			if(p < k)
			{
				newN = (int) ceil(pN*1.0/2);
				rcvMetadatasize = newN * (rcvMetadatasize / pN) +
					(max(0, (rcvMetadatasize - pN * (rcvMetadatasize / pN)) -
						(pN - newN)));
			}
			else
			{
				idx = k;
				newN = pN/2;
				rcvMetadatasize = newN * (rcvMetadatasize / pN) +
					(min(newN, (rcvMetadatasize - pN * (rcvMetadatasize / pN))));
			}	// end if
			
			pN = newN;
			k = idx + (int) ceil(pN*1.0/2);
		}	// end while
		
		newN = pN/2;

		rcvMetadatasize = newN * (rcvMetadatasize / pN) +
			(min(newN, (rcvMetadatasize - pN * (rcvMetadatasize / pN))));
		pN = newN;

		MPI_Recv(metadata, 2*rcvMetadatasize, MPI_INTEGER, idx, tag, MPI_COMM_WORLD,
			&status);

		datasize = 0;
		for(int i = 1; i < 2*rcvMetadatasize; i += 2)
			datasize += metadata[i];

		MPI_Recv(data, datasize, MPI_INTEGER, idx, tag, MPI_COMM_WORLD, &status);

		metadatasize = rcvMetadatasize;
	}
	
	while (pN > 1)
	{
		newN = (int) ceil(pN*1.0/2);
		sndMetadatasize = (pN-newN) * (metadatasize / pN) +
			(min(pN-newN, metadatasize - pN * (metadatasize / pN)));

		MPI_Send(metadata+2*(metadatasize-sndMetadatasize),
			2*sndMetadatasize, MPI_INTEGER, p+newN, tag, MPI_COMM_WORLD);

		datasize = 0;
		for(int i = 2*(metadatasize-sndMetadatasize)+1; i < 2*metadatasize; i += 2)
			datasize += metadata[i];

		MPI_Send(data + metadata[2*(metadatasize-sndMetadatasize)] - metadata[0],
			datasize, MPI_INTEGER, p+newN, tag, MPI_COMM_WORLD);
		
		metadatasize = metadatasize - sndMetadatasize;
		pN = newN;
	}
	
	return metadatasize;
}

int main(int argc, char* argv[])
{
	/*inputParser ip; 
	ip.startParsers(argv[1]);

	vector<string> board = ip.getBoard();*/

	// print ou the board
	/*for(int i=0;i<board.size();i++)
	{
		cout << board[i] << endl;
	}

	cout << endl;*/

	//vector<vector<string> > pieces = ip.getPieces();

	// print out the pieces
	/*for(int i=0;i<pieces.size();i++)
	{
		for(int j=0;j<pieces[i].size();j++)
			cout << pieces[i][j] << endl;
		cout << endl;
	}*/

	/* convert tile solver to exact set cover */

	vector<int> v;
	vector<vector<int> > matrix;
	
	/*v = {1,0,1,1,1,0,0,0};
	matrix.push_back(v);
	
	v = {1,0,0,0,1,1,1,0};
	matrix.push_back(v);
	
	v = {1,0,1,0,1,1,0,0};
	matrix.push_back(v);
	
	v = {1,0,0,0,1,0,1,1};
	matrix.push_back(v);
	
	v = {0,1,1,1,0,1,0,0};
	matrix.push_back(v);
	
	v = {0,1,0,0,1,1,0,1};
	matrix.push_back(v);
	
	v = {0,1,0,1,1,1,0,0};
	matrix.push_back(v);

	v = {0,1,0,0,0,1,1,1};
	matrix.push_back(v);*/

	/*v = {0,0,1,0,1,1,0};
	matrix.push_back(v);
	
	v = {1,0,0,1,0,0,1};
	matrix.push_back(v);
	
	v = {0,1,1,0,0,1,0};
	matrix.push_back(v);
	
	v = {1,0,0,1,0,0,0};
	matrix.push_back(v);
	
	v = {0,1,0,0,0,0,1};
	matrix.push_back(v);
	
	v = {0,0,0,1,1,0,1};
	matrix.push_back(v);*/

	int vv1[] = {0,0,1,0,1,1,0};
    vector<int> v1(vv1,vv1+sizeof(vv1)/sizeof(int));
    matrix.push_back(v1);

    int vv2[] = {1,0,0,1,0,0,1};
    vector<int> v2(vv2,vv2+sizeof(vv2)/sizeof(int));
    matrix.push_back(v2);

    int vv3[] = {1,0,0,1,0,0,0};
    vector<int> v3(vv3,vv3+sizeof(vv3)/sizeof(int));
    matrix.push_back(v3);

    int vv4[] = {0,1,0,0,0,0,1};
    vector<int> v4(vv4,vv4+sizeof(vv4)/sizeof(int));
    matrix.push_back(v4);

 	int vv5[] = {0,1,0,0,0,0,1};
    vector<int> v5(vv5,vv5+sizeof(vv5)/sizeof(int));
    matrix.push_back(v5);

    int vv6[] = {0,0,0,1,1,0,1};
    vector<int> v6(vv6,vv6+sizeof(vv6)/sizeof(int));
    matrix.push_back(v6);
	

	// print out the set cover matrix
	for(int i=0;i<matrix.size();i++)
	{
		for(int j=0;j<matrix[0].size();j++)
			cout << matrix[i][j] << " ";
		cout << endl;
	} 

	/* dancing link */
	double  stime = 0.0,
			seqetime = 0.0,
    		etime = 0.0,
            mytime = 0.0,
            maxtime = 0.0;

	int	size;
	MPI_Request	request;
	MPI_Status	status;

    int	id,
		ierr,
		tag = 100;
		
/* Initialize MPI */
    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);
/* Initialization finishes */

	int MASTER = (rank == 0);
	
    stime = MPI_Wtime();

    if(MASTER)
	{
		dl = new DL(matrix);		
		search(0,initExploreLevel);
	}

	seqetime = MPI_Wtime();
	MPI_Bcast(&totalsubset, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

	totalsubset = scatterMetaData(dataset, metadata, totalsubset, size, id);

	if(MASTER)
		delete dl;

	for(int i = 0; i < 2*totalsubset; i += 2)
	{
		vector <vector<int> > subMatrix;
		int rows = dataset[metadata[i]];
		int cols = dataset[metadata[i]+1];

		cout << rows << " " << cols << endl;
		int ndata = metadata[i+1];

		//for(int j=0;j<ndata;j++)
		//	cout << dataset[metadata[i]+j];
		//cout << endl;

		for(int j=0;j<cols;j++)
			for(int k=0;k<rows;k++)
			{
				int temp = dataset[metadata[i]+2+j*rows+k];
				//cout << metadata[i]+2+j*cols+k << " "<< temp << endl;
				if(j==0) 
				{
					vector<int> tempv;
					tempv.push_back(temp);
					subMatrix.push_back(tempv);
				}
				else
				{
					subMatrix[k].push_back(temp);
				}
			}

		dl = new DL(subMatrix);
		dl->search(0);
		delete dl;
		//cout << endl;
		//runs for the subset problem

		//cout << subMatrix.size() << endl;
		//cout << subMatrix[1].size() << endl;
		/*for(int j=0;j<subMatrix.size();j++)
		{
			for(int k=0;k<subMatrix[1].size();k++)
				cout << subMatrix[j][k] << " ";
			cout << endl;
		}*/
	}

	cout << totalSolutions << endl;
	int grandTotalSolutions = 0;
	MPI_Reduce(&totalSolutions, &grandTotalSolutions, 1, MPI_INTEGER,
		MPI_SUM, 0, MPI_COMM_WORLD);
	
	etime = MPI_Wtime();
    mytime = etime - stime;

    MPI_Reduce(&mytime, &maxtime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (MASTER)
    {
    	cout << "Max processing at any node is " << maxtime << " second(s)\n";
    	cout << "Sequenctial processing time is " <<     seqetime - stime << " second(s)\n";
    }

	if(MASTER)
		cout << "Total " << grandTotalSolutions << " solution(s) found\n";
	
	MPI_Finalize();
    return 0;
}
