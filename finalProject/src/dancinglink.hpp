#include <iostream>
#include <vector>

using namespace std;

struct Node
{
	Node* left, *right, *up, *down;
	int col, row;
	Node(){
		left = NULL; right = NULL;
		up = NULL; down = NULL;
		col = 0; row = 0;
	}
	Node( int r, int c )
	{
		left = NULL; right = NULL;
		up = NULL; down  = NULL;
		col = c; row = r;
	}
};

struct ColunmHeader : public Node
{
	int size;
	ColunmHeader(){
		size = 0;
	}
};

class DL
{
public:
	int ROWS, COLS;
 	DL(vector<vector<int> > matrix);
	~DL();
	void insert(int r, int c);
	void cover(int c);
	void uncover(int c);
	bool search(int k, int max=-1);
	int getFound() { return totalSolutionsFound; }
	void pushPartial(int i) { resultStack.push_back(i); }
	void checkRows(vector<int> rows);
private:
	int getData(int a[], int k);

private:
	ColunmHeader* root;
	ColunmHeader* ColIndex;
	Node* RowIndex;
	int totalSolutionsFound;
	vector<int> resultStack;
};
