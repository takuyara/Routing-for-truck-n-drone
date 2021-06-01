//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#pragma once
#include "config.h"
#include "utils.h"
#include <string>
class BaseData{
public:
	//Vertices:
	int V;
	Point vertex[maxv];
	//Edges:
	int E;
	//Trucks:
	int M, L;
	// edge i: (u, v) -> (w_i, t_i)
	// (u, v) and (v, u) are considered as different edges.
	EdgeMap edge;
	//Drones:
	double S, R;
	//Customers:
	double P;
	int N;
	int z[maxn];
	double s[maxn], t[maxn];
	//Warehouse
	int Z;
	//About implements:
	void SaveToFile(const std::string &);
	void ReadFromFile(const std::string &);
} ;

