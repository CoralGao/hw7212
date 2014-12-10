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
		//int isIn = -1;
		vector <vector <string> > allDifferent = getAllDiff(m_pieces[i]);

		//cout << allDifferent.size() << endl;
		
		/*for(int j=0;j<allDifferent.size();j++)
		{
			for(int k=0;k<allDifferent[j].size();k++) cout << allDifferent[j][k] << endl;
			cout << endl;
		}*/

		//vector<string> temp = allDifferent[0];

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
	// find how many pieces are the same.
	// color the pieces.
	// vector <vector<string> > pieces, vector<string> board,
	// get the mapping information of the board and the matrix
	map <int, pair<int,int> > boardMatrx;
	vector<string> tempBoard;
	map <vector<vector<string> >, char> colorBoard;
	map <int, char> colorPiece;
	vector<vector<string> > diffResult;
	char sColor = 'a'-1;
	int isIn, c = 0;

	// map board with the matrix
	for(int i=0;i<m_board.size();i++)
	{
		string t="";
		for(int j=0;j<m_board[i].size();j++)
		{
			if(m_board[i][j]!=' ')
				t+= 'a'-1;
			else
				t+= ' ';
		}
		tempBoard.push_back(t);
	}	

	cout << "the tempBoard  is: " << endl;	
	//cout the tempBoard
	for(int i=0;i<m_board.size();i++)
	{
		for(int j=0;j<m_board[i].size();j++)
			cout << tempBoard[i][j] << " " ;
		cout << endl;
	}

	for(int i=0;i<tempBoard.size();i++)
	{
		for(int j=0;j<tempBoard[i].size();j++)
		{
			if(tempBoard[i][j]!=' ')
			{
				boardMatrx[c] = make_pair(i,j);
				c++;
			}	
		}
	}
	
	// map piece with the color

	for(int i=0;i<m_pieces.size();i++)
	{
		vector <vector <string> > allDifferent = getAllDiff(m_pieces[i]);
		vector <string> tempPiece = allDifferent[0];
		isIn = -1;
		for(map <vector<vector<string> >, char>::iterator it = colorBoard.begin();it!=colorBoard.end();it++)
		{
			if(isInSet(tempPiece,it->first)){
				colorPiece[i] = it->second;
				isIn = 0;
				break;
			}
		}
		if(isIn == -1){
			sColor++;
			colorBoard[allDifferent] = sColor;
			colorPiece[i] = sColor;
		}
	}
	//cout << "sColor is: " << sColor << endl;

	//check all the results;
	for(int i=0;i<size;i++)
	{
                vector <string> tempResult= tempBoard;
		int n = data[i];
		for(int j=0;j<n;j++){
			//cout << "comes to get the piece" << endl;
			int np, nl = data[i+1], posI, posJ;
			i++;
			char tc;
			/*for(int ii=0;ii<m[nl].size();ii++)
			{
				cout << m[nl][ii] << " ";
			}				
			cout << endl;*/

			for(int k=0;k<m_pieces.size();k++){
				//cout << k << endl;
				if(m[nl][k]==1) {np = k; break;}
			}

			//cout << "board number is: " << np << endl;
			
			tc = colorPiece[np];
			//cout << "temp color is " << tc << endl;
			for(int k=0;k<m[nl].size()-m_pieces.size();k++)
			{
				if(m[nl][k+m_pieces.size()]){
					pair<int,int> r = boardMatrx[k];
					posI = r.first;
					posJ = r.second;
					tempResult[posI][posJ] = tc;
					//cout << tempResult[posI][posJ] << endl;
				}
			}	
		}
	
		//cout << "comes to compare the reuslts" << endl;	
		//for(int j=0;j<tempResult.size();j++){
		//	cout << tempResult[j] << endl;
		//}
		//cout << endl;

		vector <vector<string> > tempDiffResult = getAllDiff(tempResult);
		isIn = -1;

		//cout << "goes to compare the result" << endl;
		//cout << "different result size is: " << diffResult.size() << endl;
		for(int j=0;j<tempDiffResult.size();j++){
			if(isInSet(tempDiffResult[j],diffResult)){
				isIn = 0;
				//cout << "leaves it." << endl;
				break;
			}
		}
		
		if(isIn == -1){
			diffResult.push_back(tempDiffResult[0]);
		}
	}
	
	return diffResult.size();
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
				else if(m_board[j][k]!= ' ') t.push_back(0);
		}

		m_set.push_back(t);
	}
}

/*void
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
}*/
