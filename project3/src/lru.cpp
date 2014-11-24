#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

class LRU 
{
public:
	LRU(int size)
	:m_size(size)
	{
	}

	void put(int a)
	{
		if(m_list.size()>=m_size)
		{
			m_list.pop_front();
		}
		m_list.push_back(a);
	}

	bool get(int a)
	{
		list<int>::iterator findIt = find(m_list.begin(),m_list.end(),a);
		if(findIt != m_list.end())
		{
			m_list.splice(m_list.end(),m_list,findIt);
			return true;
		}
		else
			return false;
	}

	int getSize() {return m_list.size();}

private:
	int m_size;
	list<int> m_list;
};