/*
Author: Minsheng Zhang
		Shanshan Gao
*/

#include "parserInput.hpp"
#include "tileToSetCover.hpp"
#include "dancinglink.hpp"
#include <iostream>
#include <vector>

using namespace std;

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

	/* dancing link */
	/*vector<int> v;
	vector<vector<int> > matrix;
	
	v = {1,0,1,1,1,0,0,0};
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
	matrix.push_back(v);

	// print out the set cover matrix
	for(int i=0;i<matrix.size();i++)
	{
		for(int j=0;j<matrix[0].size();j++)
			cout << matrix[i][j] << " ";
		cout << endl;
	} 

	DL dl(matrix);
	//dl.matrixcvtLinks(matrix);
	int a = dl.search();
	cout << a << endl;*/
}
