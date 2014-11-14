/*
Author: Minsheng Zhang
		Shanshan Gao
*/

#include <vector>
#include <list>
#include <string>

using namespace std;

class inputParser
{
public:
	bool startParsers(string fileName);

	vector <vector<string> >
	getPieces()
	{
		return m_pieces;
	}

	vector <string>
	getBoard()
	{
		return m_board;
	}

private:
	int length(string s);

private:
	void
	parsePieces(vector<string> input,int width);

	vector<string> m_board;
	vector <vector<string> > m_pieces;

};