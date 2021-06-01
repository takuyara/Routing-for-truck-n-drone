//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#define n_iter 5000
#include "Init.h"
#include "DroneGet.h"
#include "SolSet.h"
#include "BaseSolC.h"
#include "pdata.h"
#include "AdjGet.h"
#include <algorithm>
using pdata::dt;
bool iscity(int i){
	return std::abs(dt.vertex[i].x) < 5000 & std::abs(dt.vertex[i].y) < 5000;
}
int main(){
	//puts("Before rd");
	dt.ReadFromFile(config::input_fname);
	//puts("After rd");
	//puts("p1 newsol done");
	//printf("p1 addr = %x\n", p1);
	//p1->NewSol();
	FILE *f1 = fopen("line.txt", "w");
	
	BaseInit *p1 = (BaseInit*)(new GreedyInit());
	BaseSolSet *solset = new S1SolSet({p1});
	BaseDroneGet *dget = new DroneGet1();
	BaseAdjGet *aget = new AdjGet1();
	for (int i = 1; i <= n_iter; ++i){
		//printf("Iteration No. %d\n", i);
		BaseSolSet *s1 = solset->Iteration(aget, dget);
		delete solset;
		solset = s1;
		//solset->gbest()->ShowResult(&pdata::dt);
		double ans = solset->gbest()->ActualCost(&pdata::dt);
		//double ans1 = solset->gcur()->ActualCost(&pdata::dt);
		fprintf(f1, "%.2lf\n", ans);
		printf("Iteration %d, best = %.2lf\n", i, ans);
		if (i % 100 == 0) solset->gbest()->SaveToFile("solution_tmp.txt");
	}
	
	solset->gbest()->SaveToFile(config::sol_fname);
	solset->gbest()->ShowResult(&pdata::dt);
	solset->gbest()->SaveToFile("solution.txt");
	/*
	bool vis[maxv] = {0};
	vis[dt.Z] = true;
	std::vector<Route*> rts;
	pdata::CalcJDis();
	for (int i = 1; i <= dt.M; ++i){
		int len = pdata::rand(30, 50), cur;
		vec p, f, c, po, t1;
		p.push_back(cur = dt.Z);
		for (int j = 1; j <= len; ++j){
			po.clear();
			for (int p = pdata::e[cur], t; ~p; p = pdata::d[p].n)
				if (iscity(t = pdata::d[p].y)) po.push_back(t);
			cur = po[pdata::rand(0, po.size() - 1)];
			p.push_back(cur);
		}
		p.push_back(dt.Z);
		puts("P generate");
		int cnt = pdata::rand(5, 8), t;
		for (int j = 1; j <= cnt; ++j){
			while (vis[t = pdata::rand(1, dt.V)]) ;
			f.push_back(pdata::rand(1, len));
			c.push_back(t);
		}
		rts.push_back(new Route(p, t1, f, c));
		puts("rst get");
	}
	Solution sol(rts);
	sol.SaveToFile("solution.txt");
	*/
	return 0;
}
	
