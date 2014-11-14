#include "parserInput.hpp"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
	inputParser ip; 
	ip.startParsers(argv[1]);

	vector<string> board = ip.getBoard();
	for(int i=0;i<board.size();i++)
	{
		cout << board[i] << endl;
	}

	vector<vector<string> > pieces = ip.getPieces();
	for(int i=0;i<pieces.size();i++)
	{
		for(int j=0;j<pieces[i].size();j++)
			cout << pieces[i][j] << endl;
	}
}
