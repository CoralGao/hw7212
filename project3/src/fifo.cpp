#include <iostream>
#include <list>
#include <map>

using namespace std;

class FIFO
{
public:
	FIFO(int size)
	:m_size(size)
	{
		for(int i=0;i<10;i++)
			m_map[i] = false;
	}

	void put(int a)
	{
		if(m_list.size()>=m_size)
		{
			m_map[m_list.front()] = false;
			m_list.pop_front();
		}
		m_list.push_back(a);
		m_map[a] = true;

	}

	bool get(int a)
	{
		if(m_map[a]==true)
			return true;
		else 
			return false;
	}

	int getSize() {return m_list.size();}

private:
	int m_size;
	list<int> m_list;
	map<int,bool> m_map;
};