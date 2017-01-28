#pragma once
#include <vector>
#include "vertex_metadata.h"
class Heart
{
private:
	std::vector<Point> points;
	int numSteps;
public:
	Heart(int steps);
	Point* generatePoints();
	int getNumberOfPoints();
};