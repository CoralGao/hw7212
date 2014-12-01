/*
Author: Minsheng Zhang
		Shanshan Gao
*/

#include <iostream>
#include <vector>

using namespace std;

class TileToSC{
public:
	TileToSC(){};

	TileToSC(vector<vector<string> > pieces,vector<string> board)
	: m_pieces(pieces)
	, m_board(board)
	{
	}

	bool
	startConvert();
	
	vector <vector<int> >
	getSet()
	{
		return m_set;
	}

private:
	vector <vector<string> >
	getAllDiff(vector<string> p);

	vector <string>
	rotate(vector<string> p);

	vector <string>
	reflect(vector<string> p);

	bool
	isSame(vector<string> a, vector<string> b);

	bool
	isInSet(vector<string> p, vector<vector<string> > s);

	vector<vector<string> >
	insertTile(vector<vector<string> > pieces);

	vector<vector<string> >
	insertTile(vector<vector<string> > pieces,  vector<vector<string> > board);

	vector <string>
	insert(int r,int c, vector<string> p);

	vector <string>
	insert(int r,int c, vector<string> p, vector<vector<string> > board);

	bool
	isDuplicate(vector<string> t, vector<vector<string> > r);

	void
	generateResultSet(int n, vector<vector <string> > r);

	void
	generateResultSet(int ind,int n, vector<vector <string> > r);


private:
	vector<vector<string> > m_pieces;
	vector<string> m_board;
	vector <vector<int> > m_set;
};