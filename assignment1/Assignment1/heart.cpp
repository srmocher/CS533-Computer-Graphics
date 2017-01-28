#include "heart.h"
#include <cmath>

Heart::Heart(int steps) {
	this->numSteps = steps;
}

Point* Heart::generatePoints() {
	const double PI = acos(-1);
	double step_size = (2 * PI) / this->numSteps;

	float alpha = 0.0;
	while (alpha <= 2 * PI) {
		GLfloat x = (16.0 / 17.0)*sinf(alpha)*sinf(alpha)*sinf(alpha);
		GLfloat y = 13.0*cosf(alpha) - 5.0*cosf(2.0*alpha) - 2.0*cosf(3.0*alpha);
		y = y / 17.0;
		Point p;
		p.coord[0] = x;
		p.coord[1] = y;
		this->points.push_back(p);
		alpha += step_size;
	}
	
	Point *pts = new Point[this->points.size()];
	for (auto i = 0;i < this->points.size();i++) {
		Point temp;
		temp.coord[0] = this->points[i].coord[0];
		temp.coord[1] = this->points[i].coord[1];
		pts[i] = temp;
	}
	return pts;
}

int Heart::getNumberOfPoints() {
	int size = this->points.size();
	return size;
}
