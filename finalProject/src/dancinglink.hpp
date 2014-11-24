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
		int search(/* int k=0 */);

	private:
		ColunmHeader* root;
		ColunmHeader* ColIndex;
		Node* RowIndex;
};
