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

	/*int vv1[] = {0,0,1,0,1,1,0};
	vector<int> v1(vv1,vv1+sizeof(vv1)/sizeof(int));
	matrix.push_back(v1);

	int vv2[] = {1,0,0,1,0,0,1};
	vector<int> v2(vv2,vv2+sizeof(vv2)/sizeof(int));
	matrix.push_back(v2);

	int vv3[] = {0,1,1,0,0,1,0};
	vector<int> v3(vv3,vv3+sizeof(vv3)/sizeof(int));
	matrix.push_back(v3);

	int vv4[] = {1,0,0,1,0,0,0}; 
	vector<int> v4(vv4,vv4+sizeof(vv4)/sizeof(int));
	matrix.push_back(v4);

	int vv5[] = {0,1,0,0,0,0,1};
	vector<int> v5(vv5,vv5+sizeof(vv5)/sizeof(int));
	matrix.push_back(v5);

	int vv6[] = {0,0,0,1,1,0,1};
	vector<int> v6(vv6,vv6+sizeof(vv6)/sizeof(int));
	matrix.push_back(v6);*/

	/*int vv1[] = {1,0};
    vector<int> v1(vv1,vv1+sizeof(vv1)/sizeof(int));
    matrix.push_back(v1);

    int vv2[] = {1,1};
    vector<int> v2(vv2,vv2+sizeof(vv2)/sizeof(int));
    matrix.push_back(v2);

    int vv3[] = {0,1};
    vector<int> v3(vv3,vv3+sizeof(vv3)/sizeof(int));
    matrix.push_back(v3);*/

	
	int vv1[] = {1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0};
    vector<int> v1(vv1,vv1+sizeof(vv1)/sizeof(int));
    matrix.push_back(v1);

    int vv2[] = {1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0};
    vector<int> v2(vv2,vv2+sizeof(vv2)/sizeof(int));
    matrix.push_back(v2);

    int vv3[] = {1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0};
    vector<int> v3(vv3,vv3+sizeof(vv3)/sizeof(int));
    matrix.push_back(v3);

    int vv4[] = {1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0};
    vector<int> v4(vv4,vv4+sizeof(vv4)/sizeof(int));
    matrix.push_back(v4);

    int vv5[] = {1,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0};
    vector<int> v5(vv5,vv5+sizeof(vv5)/sizeof(int));
    matrix.push_back(v5);

    int vv6[] = {1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0};
    vector<int> v6(vv6,vv6+sizeof(vv6)/sizeof(int));
    matrix.push_back(v6);

    int vv7[] = {1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0};
    vector<int> v7(vv7,vv7+sizeof(vv7)/sizeof(int));
    matrix.push_back(v7);
     		
    int vv8[] = {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0};
    vector<int> v8(vv8,vv8+sizeof(vv8)/sizeof(int));
    matrix.push_back(v8);
     		
    int vv9[] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1};
    vector<int> v9(vv9,vv9+sizeof(vv9)/sizeof(int));
    matrix.push_back(v9);

    int vv11[] = {0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0};
    vector<int> v11(vv11,vv11+sizeof(vv11)/sizeof(int));
    matrix.push_back(v11);

    int vv12[] = {0,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0};
    vector<int> v12(vv12,vv12+sizeof(vv12)/sizeof(int));
    matrix.push_back(v12);

    int vv13[] = {0,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0};
    vector<int> v13(vv13,vv13+sizeof(vv13)/sizeof(int));
    matrix.push_back(v13);

    int vv14[] = {0,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0};
    vector<int> v14(vv14,vv14+sizeof(vv14)/sizeof(int));
    matrix.push_back(v14);

    int vv15[] = {0,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0};
    vector<int> v15(vv15,vv15+sizeof(vv15)/sizeof(int));
    matrix.push_back(v15);

    int vv16[] = {0,1,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0};
    vector<int> v16(vv16,vv16+sizeof(vv16)/sizeof(int));
    matrix.push_back(v16);

    int vv17[] = {0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0};
    vector<int> v17(vv17,vv17+sizeof(vv17)/sizeof(int));
    matrix.push_back(v17);
     		
    int vv18[] = {0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0};
    vector<int> v18(vv18,vv18+sizeof(vv18)/sizeof(int));
    matrix.push_back(v18);
     		
    int vv19[] = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1};
    vector<int> v19(vv19,vv19+sizeof(vv19)/sizeof(int));
    matrix.push_back(v19);

    int vv21[] = {0,0,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0};
    vector<int> v21(vv21,vv21+sizeof(vv21)/sizeof(int));
    matrix.push_back(v21);

    int vv22[] = {0,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0};
    vector<int> v22(vv22,vv22+sizeof(vv22)/sizeof(int));
    matrix.push_back(v22);

    int vv23[] = {0,0,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0};
    vector<int> v23(vv23,vv23+sizeof(vv23)/sizeof(int));
    matrix.push_back(v23);

    int vv24[] = {0,0,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0};
    vector<int> v24(vv24,vv24+sizeof(vv24)/sizeof(int));
    matrix.push_back(v24);

    int vv25[] = {0,0,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0};
    vector<int> v25(vv25,vv25+sizeof(vv25)/sizeof(int));
    matrix.push_back(v25);

    int vv26[] = {0,0,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0};
    vector<int> v26(vv26,vv26+sizeof(vv26)/sizeof(int));
    matrix.push_back(v26);

    int vv27[] = {0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0};
    vector<int> v27(vv27,vv27+sizeof(vv27)/sizeof(int));
    matrix.push_back(v27);
     		
    int vv28[] = {0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0};
    vector<int> v28(vv28,vv28+sizeof(vv28)/sizeof(int));
    matrix.push_back(v28);
     		
    int vv29[] = {0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1};
    vector<int> v29(vv29,vv29+sizeof(vv29)/sizeof(int));
    matrix.push_back(v29);

    int vv31[] = {0,0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0};
    vector<int> v31(vv31,vv31+sizeof(vv31)/sizeof(int));
    matrix.push_back(v31);

    int vv32[] = {0,0,0,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0};
    vector<int> v32(vv32,vv32+sizeof(vv32)/sizeof(int));
    matrix.push_back(v32);

    int vv33[] = {0,0,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0};
    vector<int> v33(vv33,vv33+sizeof(vv33)/sizeof(int));
    matrix.push_back(v33);

    int vv34[] = {0,0,0,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0};
    vector<int> v34(vv34,vv34+sizeof(vv4)/sizeof(int));
    matrix.push_back(v34);

    int vv35[] = {0,0,0,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0};
    vector<int> v35(vv35,vv35+sizeof(vv35)/sizeof(int));
    matrix.push_back(v35);

    int vv36[] = {0,0,0,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0};
    vector<int> v36(vv36,vv36+sizeof(vv36)/sizeof(int));
    matrix.push_back(v36);

    int vv37[] = {0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0};
    vector<int> v37(vv37,vv37+sizeof(vv37)/sizeof(int));
    matrix.push_back(v37);
     		
    int vv38[] = {0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0};
    vector<int> v38(vv38,vv38+sizeof(vv38)/sizeof(int));
    matrix.push_back(v38);
     		
    int vv39[] = {0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1};
    vector<int> v39(vv39,vv39+sizeof(vv39)/sizeof(int));
    matrix.push_back(v39);

	/*int vv1[] = {1,0,1,1,0,0,1,1,0,0,0};
    vector<int> v1(vv1,vv1+sizeof(vv1)/sizeof(int));
    matrix.push_back(v1);

	int vv2[] = {0,1,0,0,1,1,0,0,1,1,0};
    vector<int> v2(vv2,vv2+sizeof(vv2)/sizeof(int));
    matrix.push_back(v2);

    int vv4[] = {1,0,0,0,1,1,0,0,1,1,0};
    vector<int> v4(vv4,vv4+sizeof(vv4)/sizeof(int));
    matrix.push_back(v4);

    int vv5[] = {0,1,1,1,0,0,1,1,0,0,0};
    vector<int> v5(vv5,vv5+sizeof(vv5)/sizeof(int));
    matrix.push_back(v5);

	int vv6[] = {0,0,0,0,0,0,0,0,0,0,1};
    vector<int> v6(vv6,vv6+sizeof(vv6)/sizeof(int));
    matrix.push_back(v6);*/



	// print out the set cover matrix
	/*for(int i=0;i<matrix.size();i++)
	{
		for(int j=0;j<matrix[0].size();j++)
			cout << matrix[i][j] << " ";
		cout << endl;
	}*/
	

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
		cout << "Sequenctial processing time is " <<     seqetime - stime << " second(s)\n";
	}

	if(MASTER)
		cout << "Total " << grandTotalSolutions << " solution(s) found\n";
	
	MPI_Finalize();
   	 return 0;
}
