/*
Author: Minsheng Zhang
		Shanshan Gao
*/

#include <iostream>
#include <vector>

using namespace std;

class TileToSC{
public:
	int
	startConvert(vector<vector<string> > pieces,vector<string> board);
	
	vector <vector<int> >
	getSet()
	{
		return m_set;
	}

private:
	vector <vector<int> > m_set;
};