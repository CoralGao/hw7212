/*
Author: Minsheng Zhang
		Shanshan Gao
*/
		
#include "parserInput.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <queue>

using namespace std;

bool  
inputParser::startParsers(string fileName)
{
	ifstream inputFile;
	inputFile.open(fileName.c_str());
	string tmp;
	vector <string> inputVec;
	
	int height = 0;
	int width = 0;
	while (getline(inputFile,tmp))
	{
		inputVec.push_back(tmp);
		width = std::max(width,length(tmp));
		height++;
	}

	if(width == 0)
	{
		cerr << "The input file has no dominoes." << endl;
		return false;
	}	
	else
	{
		parsePieces(inputVec,width);
		return true;
	}
}

void
inputParser::parsePieces(vector<string> input,int width)
{	
	int colorMatrix[input.size()][width];
	/* initialize the searched matrix to false*/
	for(int i=0;i<input.size();i++)
		for(int j=0;j<width;j++)
		{
			colorMatrix[i][j] = 0;
 		}

 	int n = 1; /* color the pieces */
 	int puzzleNum = -1;
 	int maxNumPiece = -1;

	for(int i=0;i<input.size();i++)
		for(int j=0;j<width;j++)
		{
			if(colorMatrix[i][j]) continue;
			else if(j>=input[i].size() || input[i][j] == ' ') {colorMatrix[i][j] = -1; continue;}
			else
			{
				//get the piece from that point.
				//use a queue to search for the tree.
				queue<pair<int,int> > searchedQueue;
				searchedQueue.push(make_pair(i,j));
				int minX,minY,maxX,maxY, tempNumPiece;
				minX = maxX = i;
				minY = maxY = j;
				colorMatrix[i][j] = n;
				tempNumPiece = 0;

				while(!searchedQueue.empty())
				{
					tempNumPiece++;

					pair<int, int> tempPosition = searchedQueue.front();
					int tempX = tempPosition.first;
					int tempY = tempPosition.second;

					// upper one.
					if(tempX-1>=0 && !colorMatrix[tempX-1][tempY])
					{
						if(input[tempX-1].size()>tempY && input[tempX-1][tempY] != ' ')
						{
							colorMatrix[tempX-1][tempY] = n;
							searchedQueue.push(make_pair(tempX-1,tempY)); 
							if(minX > tempX-1) minX = tempX-1;
						}
						else
							colorMatrix[tempX-1][tempY] = -1;
					}
					// lower one.
					if(tempX+1<input.size() && !colorMatrix[tempX+1][tempY])
					{
						if(input[tempX+1].size()>tempY && input[tempX+1][tempY] != ' ')
						{
							colorMatrix[tempX+1][tempY] = n;
							searchedQueue.push(make_pair(tempX+1,tempY));
							if(maxX < tempX+1) maxX = tempX+1;
						}
						else
							colorMatrix[tempX+1][tempY] = -1;
					}
					// right one.
					if(tempY+1<width && !colorMatrix[tempX][tempY+1])
					{
						if(tempY+1 < input[tempX].size() && input[tempX][tempY+1] != ' ')
						{
							colorMatrix[tempX][tempY+1] = n;
							searchedQueue.push(make_pair(tempX,tempY+1));
							if(maxY < tempY+1) maxY = tempY+1;
						}
						else
							colorMatrix[tempX][tempY+1] = -1;
					}
					// left one.
					if(tempY-1 >=0 && !colorMatrix[tempX][tempY-1])
					{
						if(input[tempX][tempY-1] != ' ')
						{
							colorMatrix[tempX][tempY-1] = n;
							searchedQueue.push(make_pair(tempX,tempY-1));
							if(minY > tempY-1) minY = tempY-1;
						}
						else
							colorMatrix[tempX][tempY-1] = -1;
					}
					
					searchedQueue.pop();
				}
				//get the new piece;
				vector<string> newPiece;
				for(int p=minX;p<=maxX;p++)
				{
					string temp = "";
					for(int q=minY;q<=maxY;q++)
					{
						if(colorMatrix[p][q] == n)
							temp = temp + input[p][q];
						else
							temp = temp+ ' ';
					}
					newPiece.push_back(temp);
				}

				// update the puzzle
				if(maxNumPiece < tempNumPiece) {maxNumPiece=tempNumPiece;puzzleNum = n-1;}

				for(int i=0;i<newPiece.size();i++)
					for(int j=newPiece[i].size()-1;j>=0;j--)
						if(newPiece[i][j]==' '){
							newPiece[i] = newPiece[i].substr(0,newPiece[i].size()-1);
						}
						else break;
				m_pieces.push_back(newPiece);
			}
			n++;
		}

	m_board = m_pieces[puzzleNum];

	m_pieces.erase(m_pieces.begin()+puzzleNum);
}

int
inputParser::length(string s)
{
	int n = s.size();

	for(int i=0;i<s.size();i++)
	{
		if(s[s.size()-1-i]==' ')
			n--;
		else
			break;
	}

	return n;
}