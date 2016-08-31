#include <stdio.h>
#include <map>
#include <list>

enum Priority
{
	First,
	Second,
	Third,
};

void main()
{
	typedef std::list< std::string > strList;

	std::map< Priority, strList, std::less< Priority > > map;

	strList secondList;
	secondList.push_back("2");
	map.insert(std::make_pair(Second, secondList));

	strList thirdList;
	thirdList.push_back("3");
	map.insert(std::make_pair(Third, thirdList));

	strList firstList;
	firstList.push_back("1_1");
	firstList.push_back("1_2");
	map.insert(std::make_pair(First, firstList));

	for (const auto &m : map)
	{
		for (const auto &s : m.second)
		{
			printf("%s\n", s.c_str());
		}
	}

	getchar();
}