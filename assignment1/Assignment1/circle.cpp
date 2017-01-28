#include<vector>
#include<cmath>
#include <GL3/gl3.h>
#include "circle.h"

Circle::Circle(double r, int s) {
	this->radius = r;
	this->steps = s;
}
GLfloat** Circle::generatePoints() {
	double radius = this->radius;
	int steps = this->steps;
	
	const double PI = atan(1) * 4;
	double step_size = (2 * PI) / steps;

	double alpha = 0.0;
	GLfloat *temp = new GLfloat[2];
	temp[0] = 0.0;
	temp[1] = 0.0;
	this->points.push_back(temp);
	while (alpha <= 2 * PI) {
		GLfloat x = radius * cos(alpha);
		GLfloat y = radius * sin(alpha);
		GLfloat *point = new GLfloat[2];
		point[0] = x;
		point[1] = y;
		this->points.push_back(point);
		alpha += step_size;
	}

	GLfloat **pointsArray = new GLfloat*[this->points.size()];
	for (auto i = 0;i < this->points.size();i++)
		pointsArray[i] = new GLfloat[4];

	for (auto j = 0;j < this->points.size();j++) {
		GLfloat *temp = this->points[j];
		pointsArray[j][0] = temp[0];
		pointsArray[j][1] = temp[1];
	}
	this->numPoints = points.size();
	return pointsArray;
}

int Circle::getNumberOfPoints() {
	return this->numPoints;
}