#include "dbscan.hpp"
#include <iostream>

struct Point
{
	int m_x;
	int m_y;
};

int distance2(const Point& a, const Point& b)
{
	return (a.m_x - b.m_x)*(a.m_x - b.m_x) + (a.m_y - b.m_y)*(a.m_y - b.m_y);
}

void expect(bool expectation)
{
	std::cout << (expectation?"succeed":"failed") << std::endl;
}

int main()
{
	const std::vector<Point> elements = {
		{0, 0},
		{2, 2},
		{2, 1},
		{1, 2},
		{4, 4},
	};
	{		
		std::cout << "simple case, 1 cluster and noise" << std::endl;
		const auto result = dbscan(elements, 1, 2, distance2);
		
		expect(result.at(0) == NOISE);
		expect(result.at(1) == 1);
		expect(result.at(2) == 1);
		expect(result.at(3) == 1);
		expect(result.at(4) == NOISE);
		std::cout << std::endl;
	}
	{
		std::cout << "increase distance, all in 1 cluster" << std::endl;
		const auto result = dbscan(elements, 4, 2, distance2);
	
		expect(result.at(0) == 1);
		expect(result.at(1) == 1);
		expect(result.at(2) == 1);
		expect(result.at(3) == 1);
		expect(result.at(4) == 1);
		std::cout << std::endl;
	}
	{
		std::cout << "increase min cluster size, all noise" << std::endl;
		const auto result = dbscan(elements, 4, 6, distance2);
	
		expect(result.at(0) == NOISE);
		expect(result.at(1) == NOISE);
		expect(result.at(2) == NOISE);
		expect(result.at(3) == NOISE);
		expect(result.at(4) == NOISE);
		std::cout << std::endl;
	}
	
	return 0;
}