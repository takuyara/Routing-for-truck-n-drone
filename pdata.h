//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#pragma once
#include <deque>
#include <random>
#include "BaseData.h"
#define maxint 1000000
#define inf 1e100
namespace pdata{
	extern std::mt19937 gen;
	extern std::uniform_int_distribution<int> unidis;
	extern std::uniform_real_distribution<double> unidisf;
	extern BaseData dt;
	extern bool calc_jdis;
	extern double jdis[maxn][maxv], jtime[maxn][maxv];
	extern int prev[maxn][maxv];
	extern int e[maxv];
	extern struct edge{
		int y, n;
		double c, t;
	} d[maxe];
	extern bool vis[maxv];
	extern int vtocus[maxv];
	void addedge(int x, int y, double c, double t);
	void CalcJDis();
	int rand(int x, int y);
	double rand01();
}

