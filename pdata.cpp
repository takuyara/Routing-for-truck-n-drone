//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#include "pdata.h"
namespace pdata{
	std::mt19937 gen(20170208);
	std::uniform_int_distribution<int> unidis(0, maxint);
	std::uniform_real_distribution<double> unidisf(0, 1);
	BaseData dt;
	bool calc_jdis = false;
	double jdis[maxn][maxv] = {0}, jtime[maxn][maxv] = {0};
	int prev[maxn][maxv];
	int e[maxv];
	struct edge d[maxe];
	bool vis[maxv];
	int vtocus[maxv];
	
void addedge(int x, int y, double c, double t){
	static int l = 0;
	d[l] = (edge){y, e[x], c, t}, e[x] = l++;
}
void CalcJDis(){
	if (calc_jdis) return ;
	calc_jdis = true;
	puts("Jonathan in");
	for (int i = 1; i <= dt.N; ++i) vtocus[dt.z[i]] = i;
	vtocus[dt.Z] = 0;
	for (int i = 1; i <= dt.V; ++i) e[i] = -1;
	for (auto i: dt.edge)
		addedge(i.first.first, i.first.second, i.second.first, i.second.second); 
	bool printme;
	for (int i = 0; i <= dt.N; ++i){
		int u = i ? dt.z[i] : dt.Z;
		printme = i == 28;
		for (int j = 1; j <= dt.V; ++j)
			vis[j] = false, jdis[i][j] = inf, prev[i][j] = -1;
		std::deque<int> s;
		vis[u] = true, s.push_back(u), jdis[i][u] = jtime[i][u] = 0, prev[i][u] = 0;
		while (!s.empty()){
			int v = s.front();
			//if (printme) printf("cus 28: to %d\n", v);
			for (int p = e[v], t; ~p; p = d[p].n)
				if (jdis[i][t = d[p].y] > jdis[i][v] + d[p].c){
					jdis[i][t] = jdis[i][v] + d[p].c;
					jtime[i][t] = jtime[i][v] + d[p].t;
					prev[i][t] = v;
					s.push_back(t);
					vis[t] = true;
				}
			s.pop_front();
			vis[v] = false;
		}
	}
	puts("Jonathan out");
}
int rand(int x, int y){
	return x == y ? x : (unidis(gen) % (y - x + 1) + x);
}
double rand01(){
	return unidisf(gen);
}
}
