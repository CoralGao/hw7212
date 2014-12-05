/*
Author: Minsheng Zhang
		Shanshan Gao
*/

#include "tileToSetCover.hpp"
#include <vector>
#include <map>

using namespace std;

bool
TileToSC::startConvert()
{
	for(int i=0;i<m_pieces.size();i++)
	{
		// add the tempSet to the m_set
		int isIn = -1;
		vector <vector <string> > allDifferent = getAllDiff(m_pieces[i]);

		//cout << allDifferent.size() << endl;
		
		/*for(int j=0;j<allDifferent.size();j++)
		{
			for(int k=0;k<allDifferent[j].size();k++) cout << allDifferent[j][k] << endl;
			cout << endl;
		}*/

		vector<string> temp = allDifferent[0];

		/*for(map<int, vector<vector<string> > >::iterator it=pieceMap.begin(); it!=pieceMap.end(); ++it)
		{
			if(isInSet(allDifferent[0],it->second)){
				isIn = it->first;
				break;
			}
		}

		if(isIn == -1)
			pieceMap[i] = allDifferent;*/

		//if(isIn == -1){
		vector<vector <string> > tempSet = insertTile(allDifferent);
		generateResultSet(i,tempSet);
			/*for(int ii=0;ii<tempSet.size();ii++)
			{
				for(int jj=0;jj<tempSet[ii].size();jj++)
					cout << tempSet[ii][jj] << endl;
				cout << endl;
			}*/
			//cout << tempSet.size() << endl;
		//} else {
		//	vector<vector <string> > tempSet = insertTile(allDifferent,resultMap[isIn]);
		//	generateResultSet(isIn,i,tempSet);
		//	resultMap[isIn] = tempSet;
		//}
	}

	if(m_set.size()!=0) return true;
	else return false;
}

int
TileToSC::getDiffResult(int *data, int size, vector<vector<int> >m)
{
	return 1;
}

vector <vector<string> >
TileToSC::getAllDiff(vector<string> p)
{
	vector <vector<string> > result;
	vector <string> temp;

	result.push_back(p);

	temp = rotate(p);
	if(!isInSet(temp,result)) result.push_back(temp);

	temp = rotate(temp);
	if(!isInSet(temp,result)) result.push_back(temp);

	temp = rotate(temp);
	if(!isInSet(temp,result)) result.push_back(temp);

	temp = reflect(p);
	if(!isInSet(temp,result)) result.push_back(temp);

	temp = rotate(temp);
	if(!isInSet(temp,result)) result.push_back(temp);

	temp = rotate(temp);
	if(!isInSet(temp,result)) result.push_back(temp);

	temp = rotate(temp);
	if(!isInSet(temp,result)) result.push_back(temp);

	return result;
}

vector <string>
TileToSC::rotate(vector<string> p)
{
	vector<string> result;

	int max = 0;

	for(int i=0;i<p.size();i++)
		if(p[i].size()>max)
			max = p[i].size();

	for(int i=0;i<max;i++){
		string temp;
		for(int j=p.size()-1;j>=0;j--){
			if(i<p[j].size())
				temp += p[j][i];
			else
				temp += ' ';
		}

		result.push_back(temp);
	}

	for(int i=0;i<result.size();i++)
		for(int j=result[i].size()-1;j>=0;j--)
			if(result[i][j]==' '){
				result[i] = result[i].substr(0,result[i].size()-1);
			}
			else break;
	
	return result;
}

vector <string>
TileToSC::reflect(vector<string> p)
{
	vector<string> result;

	int max = 0;

	for(int i=0;i<p.size();i++)
		if(p[i].size()>max)
			max = p[i].size();

	for(int i=0;i<p.size();i++){
		string temp;
		// initiliaze the result string
		for(int j=0;j<max;j++){
			if(max-j-1>p[i].size()-1)
				temp  += ' ';
			else
				temp  += p[i][max-j-1];
		}

		result.push_back(temp);
	}

	for(int i=0;i<result.size();i++)
		for(int j=result[i].size()-1;j>=0;j--)
			if(result[i][j]==' '){
				result[i] = result[i].substr(0,result[i].size()-1);
			}
			else break;

	return result;
}

bool
TileToSC::isSame(vector<string> a, vector<string> b)
{
	if(a.size()!=b.size()) return false;

	for(int i=0;i<a.size();i++)
		if(a[i]!=b[i]) return false;

	return true;
}

bool
TileToSC::isInSet(vector<string> p, vector<vector<string> > s)
{
	for(int i=0;i<s.size();i++)
		if(isSame(s[i],p)) return true;

	return false;
}

vector<vector<string> >
TileToSC::insertTile(vector<vector<string> > pieces)
{
	vector<vector<string> > result;
	int h;

	for(int i=0;i<pieces.size();i++){
		vector<string> p = pieces[i];
		h = p.size();

		for(int j=0;j<m_board.size();j++){
			if(j+h>m_board.size()) break;

			for(int k=0;k<m_board[j].size();k++){

				vector <string> t = insert(j,k,p);
				if(t.size()==m_board.size() /*&& !isDuplicate(t,result)*/) { 
					result.push_back(t);
					/*for(int ii=0;ii<t.size();ii++)
					{
						cout << t[ii] << endl;
					}
					cout << endl;*/
				}
 			}
		}
	}

	return result;
}

vector<vector<string> >
TileToSC::insertTile(vector<vector<string> > pieces,  vector<vector<string> > boards)
{
	vector<vector<string> > result;
	int h;

	for(int ii = 0;ii<boards.size();ii++){
		vector<string> board = boards[ii];
		for(int i=0;i<pieces.size();i++){
			vector<string> p = pieces[i];
			h = p.size();

			for(int j=0;j<board.size();j++){
				if(j+h>board.size()) break;

				for(int k=0;k<board[j].size();k++){

					vector <string> t = insert(j,k,p,board);
					if(t.size()==board.size() /*&& !isDuplicate(t,result)*/) { 
						result.push_back(t);
					}
				}
			}
		}
	}

	return result;
}

vector <string>
TileToSC::insert(int r,int c, vector<string> p)
{	
	vector <string> result;
	string t="";
	int ret=1;

	for(int i=0;i<m_board.size();i++)
		result.push_back(m_board[i]);

	for(int i=0;i<p.size();i++){
		if(p[i].size()+c>m_board[r].size()) { ret = -1; break; }

		for(int j=0;j<p[i].size();j++)
			if(p[i][j]!=' ' && m_board[r+i][c+j]!=p[i][j]) {ret = -1; break;}
			else if(p[i][j]!=' ') {result[r+i][c+j] = ' ';}

		if(ret == -1) break;
	}

	if(ret == -1) result.push_back(t);

	return result;	
}

vector <string>
TileToSC::insert(int r,int c, vector<string> p, vector<string> board)
{	
	vector <string> result;
	string t="";
	int ret=1;

	for(int i=0;i<board.size();i++)
		result.push_back(board[i]);

	for(int i=0;i<p.size();i++){
		if(p[i].size()+c>board[r].size()) { ret = -1; break; }

		for(int j=0;j<p[i].size();j++)
			if(p[i][j]!=' ' && board[r+i][c+j]!=p[i][j]) {ret = -1; break;}
			else if(p[i][j]!=' ') {result[r+i][c+j] = ' ';}

		if(ret == -1) break;
	}

	if(ret == -1) result.push_back(t);

	return result;	
}

bool
TileToSC::isDuplicate(vector<string> t, vector<vector<string> > r)
{
	if(isInSet(t,r)) return true;

	t = rotate(t);
	if(isInSet(t,r)) return true;

	t = rotate(t);
	if(isInSet(t,r)) return true;

	t = rotate(t);
	if(isInSet(t,r)) return true;

	t = reflect(t);
	if(isInSet(t,r)) return true;

	t = rotate(t);
	if(isInSet(t,r)) return true;

	t = rotate(t);
	if(isInSet(t,r)) return true;

	t = rotate(t);
	if(isInSet(t,r)) return true;

	return false;
}

void
TileToSC::generateResultSet(int n, vector<vector <string> > r)
{
	for(int i=0;i<r.size();i++)
	{
		vector <string> temp = r[i];
		vector <int> t;
		for(int j=0;j<m_pieces.size();j++) t.push_back(0);
		t[n] = 1;

		for(int j=0;j<temp.size();j++)
		{
			for(int k=0;k<temp[j].size();k++)
				if(temp[j][k]!=m_board[j][k]) t.push_back(1);
				else t.push_back(0);
		}

		//for(int ii=0;ii<t.size();ii++)
		//	cout << t[ii] << " ";
		//cout << endl;

		m_set.push_back(t);
	}
}

void
TileToSC::generateResultSet(int ind, int n, vector<vector <string> > r)
{
	for(int i=0;i<r.size();i++)
	{
		vector <string> temp = r[i];
		vector <int> t;

		for(int j=0;j<m_pieces.size();j++) t.push_back(0);

		for(int j=m_set.size()-1;j>=0;j--)
		{
			if(m_set[j][ind] == 1) {
					for(int k=0;k<t.size();k++)
					{
						if(m_set[j][k]==1)  
							t[k] = 1;
					}
					break;
				}
		}

		t[n] = 1;

		for(int j=0;j<temp.size();j++)
		{
			for(int k=0;k<temp[j].size();k++)
				if(temp[j][k]!=m_board[j][k]) t.push_back(1);
				else t.push_back(0);
		}

		m_set.push_back(t);
	}
}
