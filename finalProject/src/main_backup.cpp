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
#include <algorithm>
#include <mpi.h>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <stack>

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

  		datasize = 1+metadata[2*rcvMetadatasize-1]-metadata[0];

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

		datasize = 1+metadata[2*metadatasize-1]-metadata[2*(metadatasize-sndMetadatasize)];
	
		MPI_Send(data + metadata[2*(metadatasize-sndMetadatasize)] - metadata[0],
			datasize, MPI_INTEGER, p+newN, tag, MPI_COMM_WORLD);
	
		metadatasize = metadatasize - sndMetadatasize;
		pN = newN;
	}
	
	return metadatasize;
}

int main(int argc, char* argv[])
{
	int initExploreLevel = 0;

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

	int MASTER = (id == 0);
	
    stime = MPI_Wtime();

    if(MASTER)
	{
		inputParser ip; 
		ip.startParsers(argv[1]);

		vector<string> board = ip.getBoard();
		vector <vector<string> > pieces = ip.getPieces();

		TileToSC sc(pieces,board);
		sc.startConvert();
		vector <vector<int> > matrix = sc.getSet();

		dl = new DL(matrix);		
		dl->search(0,0);
		cout <<id << " find solutions: " << totalSolutions << endl;
	}

	seqetime = MPI_Wtime();
	MPI_Bcast(&totalsubset, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

	totalsubset = scatterMetaData(dataset, metadata, totalsubset, size, id);

	//cout << "Rank "<< id << "# ";
	/*for(int i = 0; i < 2*totalsubset; i++)
		cout << metadata[i] << " ";
	cout << endl;

	cout << "Rank "<< id << "# ";
	for(int i = 0; i < metadata[2*totalsubset-2] - metadata[0] + metadata[2*totalsubset-1]; i++)
		cout << dataset[i] << " ";
	cout << endl;*/
	
	if(MASTER)
		delete dl;

	for(int i = 0; i < 2*totalsubset; i += 2)
	{
		//cout << "rank "<< id << "# " <<  i << " " << totalsubset << endl;
		vector <vector<int> > subMatrix;
		//cout << metadata[i] << " " << metadata[i+1] << endl;
		int offset = metadata[i] - metadata[0];
		int totalPartialResult = dataset[offset];

		int rows = dataset[offset];
		int cols = dataset[offset+1];

		//int ndata = metadata[i+1];

		//for(int j=0;j<ndata;j++)
		//	cout << dataset[metadata[i]+j];
		//cout << endl;

		for(int j=0;j<cols;j++)
			for(int k=0;k<rows;k++)
			{
				int temp = dataset[offset+2+j*rows+k];
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
		
		if(subMatrix.size()==0 && subMatrix[0].size()==0){
			totalSolutions++;
			// to do, cout the partial result
		}else if(subMatrix.size()!=0&&subMatrix[0].size()!=0){
			dl = new DL(subMatrix);
			dl->search(0);
			cout << id << " find solutions: " << totalSolutions << endl;
			delete dl;
		}
		//cout << endl;
		//runs for the subset problem

		//cout << subMatrix.size() << endl;
		//cout << subMatrix[1].size() << endl;
		/*cout << "the matrix is: " << endl;
		for(int j=0;j<subMatrix.size();j++)
		{
			for(int k=0;k<subMatrix[0].size();k++)
				cout << subMatrix[j][k] << " ";
			cout << endl;
		}*/
	}

	//cout << totalSolutions << endl;
	int grandTotalSolutions = 0;
	MPI_Reduce(&totalSolutions, &grandTotalSolutions, 1, MPI_INTEGER,
		MPI_SUM, 0, MPI_COMM_WORLD);
	
	etime = MPI_Wtime();
	mytime = etime - stime;

	MPI_Reduce(&mytime, &maxtime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if (MASTER)
	{
		cout << "Max processing at any node is " << maxtime << " second(s)\n";
		cout << "Sequenctial processing time is " << seqetime - stime << " second(s)\n";
	}

	if(MASTER)
		cout << "Total " << grandTotalSolutions << " solution(s) found\n";
	
	MPI_Finalize();
   	 return 0;
}
