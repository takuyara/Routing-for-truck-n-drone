//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#include "utils.h"
#include "Exceptions.h"
#include "config.h"
#include <cmath>
Point::Point(): x(0), y(0){ }
Point::Point(double x1, double y1): x(x1), y(y1){ }
int toint(double x){
	if (abs(x - floor(x)) > eps) throw NonIntException();
	return x;
}
double dist(const Point &a, const Point &b){
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
Point GetNaka(const Point &a, const Point &b, double alpha){
	return Point((1 - alpha) * a.x + alpha * b.x, (1 - alpha) * a.y + alpha * b.y);
}

