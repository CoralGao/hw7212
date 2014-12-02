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
int dataset[10000000];
int metadata[MAX_PROC_NUMBER * 2];
int totalsubset = 0;
int totalSolutions = 0;
int resultSet[4];
int resultData[10000000];

DL *dl = NULL;

int scatterMetaData(int *data, int *metadata, int metadatasize, int P, int p);
void scatterResult(int *result, int *resultSet, int P, int p)

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

void 
scatterResult(int *result, int *resultSet, int P, int p)
{
	int	pN = P,
		idx = 0,
		tag = 101,
		rcvMetadatasize,
		sndMetadatasize,
		datasize,
		newN;

	bool 	isMaster = (p == 0);

	MPI_Status	status;

	if(isMaster)
	{		

		for(int i=1;i<pN;i++)
		{
			rcvMetadatasize = 2
			MPI_Recv(resultSet+2, 2, MPI_INTEGER, i, tag, MPI_COMM_WORLD,&status);

			datasize = resultSet[3]-resultSet[2];
			MPI_Recv(data+resultSet[1], datasize, MPI_INTEGER, i, tag, MPI_COMM_WORLD, &status);
			resultSet[1]+=datasize;
		}
	}

	
	if(!isMaster)
	{
		sndMetadatasize = 2;
	
		MPI_Send(resultSet, 2, MPI_INTEGER, p, tag, MPI_COMM_WORLD);

		datasize = resultSet[1]-resultSet[0];
	
		MPI_Send(resultData,datasize, MPI_INTEGER, p+newN, tag, MPI_COMM_WORLD);
	}
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

	resultSet[0] = 0; resultSet[1] = 0;

	int MASTER = (id == 0);

    stime = MPI_Wtime();

    if(MASTER)
	{
		inputParser ip; 
		ip.startParsers(argv[1]);

		vector<string> board = ip.getBoard();
		vector <vector<string> > pieces = ip.getPieces();

		cout << "Start Convert to Exact Cover.." << endl;
		TileToSC sc(pieces,board);
		sc.startConvert();
		vector <vector<int> > matrix = sc.getSet();
		/*for(int i=0;i<matrix.size();i++)
		{
			for(int j=0;j<matrix[i].size();j++)
				cerr << matrix[i][j] << " ";
			cerr << endl;
		}*/

		cout << "Finish Convert.." << endl;
		cout << "Start Master Process to search the result.."<< endl;
		dl = new DL(matrix);		
		dl->search(0,0);

		cout << "Ending the Master process search.." << endl;
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

		int partNum = dataset[offset];
		vector <int> partialResult;
		for(int ii=0;ii<partNum;ii++)
		{
			partialResult.push_back(dataset[offset+ii]);
		}
		offset += partNum+1;

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
			// put all the partial solutions into the site.
			// These partial should be put..
			for(int ii=0;ii<partialResult.size();ii++)
				dataset[resultSet[1]+ii] = partialResult[ii];
			resultSet[1] += partialResult.size()+1;

			// to do, cout the partial result
		}else if(subMatrix.size()!=0&&subMatrix[0].size()!=0){
			dl = new DL(subMatrix);

			for(int ii=0;ii<partialResult.size();ii++)
				dl->pushPartial(partialResult[ii]);

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

	scatterResult(resultSet,resultData,size,id);

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
