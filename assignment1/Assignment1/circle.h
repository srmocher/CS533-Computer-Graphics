#pragma once
#include<vector>
#include<cmath>
#include <GL3/gl3.h>
class Circle
{
private:
	std::vector<GLfloat*> points;
	double radius;
	int steps;
	int numPoints;
public:
	Circle(double r, int s);
	GLfloat** generatePoints();
	int getNumberOfPoints();
};