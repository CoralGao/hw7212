#include <iostream>
#include <queue>
#include "fifo.cpp"
#include "lru.cpp"
#include <time.h>
#include <stdlib.h> 

using namespace std;

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		cerr << "To run: ./project3 #ofReferenceString #ofFrames" << endl;
		return -1;
	}

	srand (time(NULL));
	vector <int> a;
	int num = atoi(argv[1]);
	int cacheSize = atoi(argv[2]);

	for(int i=0;i<num;i++)
	{	
		a.push_back(rand()%10);
	}

	/*for(int i=0;i<a.size();i++)
		cout << a[i] << ",";
	cout << endl;*/

 	int k1,k2; k1=0;k2=0;

 	FIFO f(cacheSize);
	for(int i=0;i<a.size();i++)
	{
		if(!f.get(a[i]))
		{
			k1++;
			f.put(a[i]);
		}
	}

	LRU l(cacheSize);
	for(int i=0;i<a.size();i++)
	{
		if(!l.get(a[i]))
		{
			k2++;
			l.put(a[i]);
		}
	}

	cout << "Page replacements percentage: " << endl;
	cout.precision(5);
	cout << "FIFO: " << k1*1.0/a.size()*100 << "%" << endl;
	cout << "LRU: " << k2*1.0/a.size()*100 << "%" << endl;

	return 0;
}