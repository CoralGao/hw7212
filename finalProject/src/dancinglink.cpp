#include "dancinglink.hpp"
#include "def.h"
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

extern int dataset[10000000];
extern int metadata[MAX_PROC_NUMBER * 2];
extern int totalsubset;
extern int totalSolutions;
extern int resultPos[2];
extern int resultSet[10000000];

DL::DL(vector<vector<int> > matrix)
{
	resultPos[0] = 0, resultPos[1] = -1;

	int totalSolutionsFound = 0;
	int rows = matrix.size();
	int cols = matrix[0].size();

	ROWS = rows;
	COLS = cols;
	
	ColIndex = new ColunmHeader[cols+1];
	RowIndex = new Node[rows];
	root = &ColIndex[0];
	ColIndex[0].left = &ColIndex[COLS];
	ColIndex[0].right = &ColIndex[1];
	ColIndex[COLS].right = &ColIndex[0];
	ColIndex[COLS].left = &ColIndex[COLS-1];
	for(int i=1; i<cols; i++)
	{
		ColIndex[i].left = &ColIndex[i-1];
		ColIndex[i].right = &ColIndex[i+1];
	}

	for (int i=0; i<=cols; i++)
	{
		ColIndex[i].up = &ColIndex[i];
		ColIndex[i].down = &ColIndex[i];
		ColIndex[i].col = i;
	}

	ColIndex[0].down = &RowIndex[0];

	for(int i=0; i<rows; i++)
		for(int j=0; j<cols; j++)
		{
			if(matrix[i][j]>0) insert(i,j+1);
		}
}

DL::~DL()
{
	for(int i=1; i<=COLS; i++)
	{
		Node* cur = ColIndex[i].down;
		Node* del = cur->down;
		while(cur != &ColIndex[i])
		{
			delete cur;
			cur = del;
			del = cur->down;
		}
	}
	delete[] RowIndex;
	delete[] ColIndex;
}

void DL::insert(int r, int c)
{
	Node* temp = &ColIndex[c];
	ColIndex[c].size++;
	Node* newNode = new Node(r, c);
	while(temp->down != &ColIndex[c] && temp->down->row < r)
		temp = temp->down;
	
	newNode->down = temp->down;
	newNode->up = temp;
	temp->down->up = newNode;
	temp->down = newNode;
	if(RowIndex[r].right == NULL)
	{
		RowIndex[r].right = newNode;
		newNode->left = newNode;
		newNode->right = newNode;
	}
	else
	{
		Node* rowHead = RowIndex[r].right;
		temp = rowHead;
		while(temp->right != rowHead && temp->right->col < c)
			temp = temp->right;
		newNode->right = temp->right;
		newNode->left = temp;
		temp->right->left = newNode;
		temp->right = newNode;
	}
}

void DL::cover(int c)
{
	ColunmHeader* col = &ColIndex[c];
	col->right->left = col->left;
	col->left->right = col->right;
	Node* tempR, *tempC;
	tempC = col->down;
	while(tempC != col)
	{
		Node* nodeR = tempC;
		tempR = nodeR->right;
		while(tempR != nodeR)
		{
			tempR->down->up = tempR->up;
			tempR->up->down = tempR->down;
			ColIndex[tempR->col].size--;
			tempR = tempR->right;
		}
		tempC = tempC->down;
	}
}

void DL::uncover(int c)
{
	Node *tempR, *tempC;
	ColunmHeader* col = &ColIndex[c];
	tempC = col->up;
	while(tempC != col)
	{
		Node* nodeR = tempC;
		tempR = tempC->left;
		while(tempR != nodeR)
		{
			ColIndex[tempR->col].size++;
			tempR->down->up = tempR;
			tempR->up->down = tempR;
			tempR = tempR->left;
		}
		tempC = tempC->up;
	}
	col->right->left = col;
	col->left->right = col;
}

bool DL::search(int k, int max)
{
	if(max != -1 && k > max && root->right != root)
	{
		if(totalsubset == 0) metadata[0] = 0;
		else metadata[totalsubset*2] = metadata[totalsubset*2-1] + 1;

		metadata[totalsubset*2+1] = metadata[totalsubset*2] + this->getData(dataset,metadata[totalsubset*2])-1;
		totalsubset++;

		return true;
	}

	if(root->right == root)
	{
		/*cout << "Find a solution:" << endl;
		for(int i=0;i<resultStack.size();i++) cout << resultStack[i] << " "; 
		cout << endl;*/
	
		resultSet[resultPos[1]+1] = resultStack.size();
		for(int i=0;i<resultStack.size();i++){
			resultSet[resultPos[1]+2+i] =resultStack[i];
		}	
		resultPos[1] += 1+resultStack.size();

		totalSolutionsFound++;
		totalSolutions++;
		return true;
	}

	ColunmHeader* choose = (ColunmHeader*)root->right, *temp=choose;
	while(temp != root)
	{
		if(choose->size > temp->size)
			choose = temp;
		temp = (ColunmHeader*)temp->right;
	}

	if(choose->size == 0)
		return false;

	cover(choose->col);

	Node* tempC = choose->down;
	while(tempC != choose)
	{
		Node* nodeR = tempC;
		resultStack.push_back(nodeR->row);		

		Node* tempR = tempC->right;

		while(tempR != nodeR)
		{
			cover(tempR->col);
			tempR = tempR->right;
		}

		search(k+1,max);

		tempR = nodeR->left;
		while(tempR != nodeR)
		{
			uncover(tempR->col);
			tempR = tempR->left;
		}
		resultStack.pop_back();
		tempC = tempC->down;
	}

	uncover(choose->col);
	return true;
}

int 
DL::getData(int a[], int k)
{
	int l = 0,
	    col = 0,
	    c = 0;
	vector<int> rows;
	map <int,int> lines;
	a[k] = resultStack.size();
	for(int i=0;i<resultStack.size();i++)
		a[k+i+1] = resultStack[i];

	k = k+1+resultStack.size();

	ColunmHeader* choose = (ColunmHeader*)root->right, *temp=choose;
	while(temp!=root)
	{
		col++;
		Node* tempC = temp->down;
		while(tempC != temp)
		{
			lines[tempC->row] = 1;
			tempC = tempC->down;
		}

		temp = (ColunmHeader*)temp->right;
	}

	for(map<int,int>::iterator it = lines.begin(); it!=lines.end(); ++it)
	{
		it->second = l;
		l++;
	}

	a[k] = lines.size();
	a[k+1] = col;
	
	//initilize
	for(int i=0;i<lines.size()*col;i++)
	{
		a[k+2+i] = 0;
	}

	temp = choose;
	while(temp!=root)
	{
		Node* tempC = temp->down;
		while(tempC!=temp)
		{
			a[k+2+lines[tempC->row]+c*lines.size()] = 1;
			rows.push_back(tempC->row);
			tempC = tempC->down;
		}
		c++;
		temp = (ColunmHeader*)temp->right;
	}

	sort(rows.begin(),rows.end());
	rows.erase(unique(rows.begin(),rows.end()),rows.end());
	//cout << "the size of the rows is: " << rows.size() << " " << rows[0] << endl;
	
	a[k+2+lines.size()*col] = rows.size();
	for(int i=0;i<rows.size();i++)
		a[k+2+lines.size()*col+i] = rows[i];

	return rows.size()+1+resultStack.size()+1+lines.size()*col+2;	
}

void
DL::checkRows(vector<int> rows)
{
	if(rows.size()==0)
		return;

	/*for(int i=0;i<rows.size();i++)
		cout << rows[i] << " ";*/

        sort(rows.begin(),rows.end());

        ColunmHeader* chooseC = (ColunmHeader*)root->right;
        while(chooseC != root)
        {
        	Node * tempC = chooseC->down;
                while (chooseC != tempC)
                {
                	tempC->row = rows[tempC->row];
                	tempC = tempC->down;
                }

                chooseC = (ColunmHeader*)chooseC->right;
        }
}
