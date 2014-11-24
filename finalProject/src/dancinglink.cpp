#include "dancinglink.hpp"
#include <vector>

using namespace std;

DL::DL(vector<vector<int> > matrix)
{
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

int DL::search()
{	
	if(root->right == root)
		return 1;

	ColunmHeader* choose = (ColunmHeader*)root->right, *temp=choose;
	while(temp != root)
	{
		if(choose->size > temp->size)
			choose = temp;
		temp = (ColunmHeader*)temp->right;
	}

	if(choose->size <= 0)
		return 0;

	int ans = 0;
	cover(choose->col);

	Node* tempC = choose->down;
	while(tempC != choose)
	{
		Node* nodeR = tempC;
		Node* tempR = tempC->right;
		while(tempR != nodeR)
		{
			cover(tempR->col);
			tempR = tempR->right;
		}

		ans += search();

		tempR = nodeR->left;
		while(tempR != nodeR)
		{
			uncover(tempR->col);
			tempR = tempR->left;
		}
		tempC = tempC->down;
	}

	uncover(choose->col);
	return ans;
}