//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#pragma once
#include <vector>
#include <utility>
#include <map>
#include <tuple>
typedef std::pair<int, int> paInt;
typedef std::pair<double, double> paDouble;
typedef std::map<paInt, paDouble> EdgeMap;
typedef std::vector<int> vec;
typedef std::vector<double> vec_d;
typedef std::tuple<const vec &, const vec &, const vec_d &, const vec &> datatup;
class Point{
public:
	double x, y;
	Point();
	Point(double, double);
} ;
double dist(const Point &, const Point &);
Point GetNaka(const Point &, const Point &, double);
int toint(double x);
template <typename T> T min(T x, T y){ return x < y ? x : y; }
template <typename T> T max(T x, T y){ return x > y ? x : y; }

