//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#include "Init.h"
#include "utils.h"
#include "pdata.h"
#include <cassert>
#include <cmath>
#include <algorithm>
GreedyInit::GreedyInit(bool f): randomize(f){
}
void insertroad(vec &a, int s, int t){
	//printf("%d %d\n", s, t);
	if (!pdata::prev[s][t]) return ;
	insertroad(a, s, pdata::prev[s][t]);
	a.push_back(t);
}
inline bool iscity(const Point &a){
	return std::abs(a.x) < 5000 && std::abs(a.y) < 5000;
}
Solution *GreedyInit::NewSolW(const std::vector<std::pair<std::pair<double, double>, int> > &v1){
	//puts("NewSolW in");
	//Calculate distance with Spfa
	using pdata::dt;
	//printf("28: (%.2lf, %.2lf)\n", dt.vertex[dt.z[28]].x, dt.vertex[dt.z[28]].y);
	//printf("94: (%.2lf, %.2lf)\n", dt.vertex[94].x, dt.vertex[94].y);
	//printf("176: (%.2lf, %.2lf)\n", dt.vertex[176].x, dt.vertex[176].y);
	//for (int p = pdata::e[28]; ~p; p = pdata::d[p].n) printf("28-%d\n", pdata::d[p].y);
		
	int curpos = dt.Z, carno = 0;
	double curtime = 0;
	std::vector<vec> p, f, c;
	vec_d time;
	p.push_back({dt.Z});
	f.push_back({});
	c.push_back({});
	time.push_back(0);
	int cuscnt = 0;
	for (auto i: v1){
		int origfsz = f.size();
		int cus = dt.z[i.second];
		//printf("cus: %d, v = %d\n", i.second, cus);
		//if (!((++cuscnt) % 50)) printf("Cus: %d\n", cuscnt);
		if (iscity(dt.vertex[cus])){
			//puts("is city");
			double mint = 1e100, jb = 0;
			//bool haveregular = false;
			for (int j = 0; j < p.size(); ++j){
				double t1 = time[j] + pdata::jtime[pdata::vtocus[p[j][p[j].size() - 1]]][cus];
				//if (time[j] < 1e5) haveregular = true;
				int lb = 0, cnt = 0;
				for (int k = p[j].size() - 1; ~k; --k) if (p[j][k] == dt.Z){ lb = k; break; }
				for (int k = f[j].size() - 1; ~k; --k) if (f[j][k] > lb) ++cnt; else break;
				if (t1 < mint && cnt < dt.L)
					if (p.size() < dt.M && p[j].size() > sugroutelen) mint = t1, jb = j;
				//if (t1 > 1e5) printf("Error, time[j] = %.2lf, jtime = %.2lf\n", time[j], pdata::jtime[pdata::vtocus[p[j][p[j].size() - 1]]][cus]);
			}
			//printf("jb = %d\n", jb);
			/*
			if (std::abs(mint - 1e100) < eps){
				if (haveregular){
					puts("How the fuck this happens?");
					for (int j = 0; j < p.size(); ++j){
						printf("Time = %.2lf, Dis = %.2lf\n", time[j], pdata::jtime[pdata::vtocus[p[j][p[j].size() - 1]]][cus]);
					}
				}
				f[jb].push_back(p[jb].size() - 1);
				c[jb].push_back(i.second);
				time[jb] = mint;
			} else
			*/ 
			if (mint > dt.t[i.second] && p.size() < dt.M){
				//puts("New route");
				p.push_back({dt.Z});
				insertroad(p[p.size() - 1], 0, cus);
				f.push_back({p[p.size() - 1].size() - 1});
				c.push_back({i.second});
				time.push_back(pdata::jtime[0][cus]);
			} else{
				//puts("
				if (std::abs(mint - 1e100) < eps){
					//Force a truck go back to warehouse.
					for (int j = 0; j < p.size(); ++j){
						double t1 = time[j] + pdata::jtime[pdata::vtocus[p[j][p[j].size() - 1]]][dt.Z];
						if (t1 < mint) mint = t1, jb = j;
					}
					insertroad(p[jb], pdata::vtocus[p[jb][p[jb].size() - 1]], dt.Z);
					mint = mint + pdata::jtime[0][cus];
				}
				//puts("Insert route");
				insertroad(p[jb], pdata::vtocus[p[jb][p[jb].size() - 1]], cus);
				//puts("Insert finish");
				f[jb].push_back(p[jb].size() - 1);
				c[jb].push_back(i.second);
				time[jb] = mint;
			}
		} else {
			//puts("is suburb");
			double mint = 1e100; int jb = 0;
			for (int j = 0; j < p.size(); ++j){
				double t1 = time[j] + dt.R * dist(dt.vertex[p[j][p[j].size() - 1]], dt.vertex[cus]);
				int lb = 0, cnt = 0;
				for (int k = p[j].size() - 1; ~k; --k) if (p[j][k] == dt.Z){ lb = k; break; }
				for (int k = f[j].size() - 1; ~k; --k) if (f[j][k] > lb) ++cnt; else break;
				if (t1 < mint && cnt < dt.L) mint = t1, jb = j;
			}
			if (std::abs(mint - 1e100) < eps){
				//Force a truck go back to warehouse.
				for (int j = 0; j < p.size(); ++j){
					double t1 = time[j] + pdata::jtime[pdata::vtocus[p[j][p[j].size() - 1]]][dt.Z];
					if (t1 < mint) mint = t1, jb = j;
				}
				insertroad(p[jb], pdata::vtocus[p[jb][p[jb].size() - 1]], dt.Z);
				mint = mint + dt.R * dist(dt.vertex[0], dt.vertex[cus]);
			}
			f[jb].push_back(p[jb].size() - 1);
			c[jb].push_back(i.second);
			time[jb] = mint;
		}
		//assert(f.size() != origfsz + 1);
	}
	//printf("psz = %d, fsz = %d, csz = %d, tsz = %d\n", p[0].size(), f[0].size(), c.size(), time.size());
	//puts("Get in solu");
	std::vector<Route*> rts;
	for (int i = 0; i < p.size(); ++i){
		vec t;
		insertroad(p[i], pdata::vtocus[p[i][p[i].size() - 1]], dt.Z);
		for (auto j: p[i]) if (j == dt.Z) t.push_back(0);
		t.pop_back();
		if (f[i].size() != c[i].size()) puts("What the hell is that?");
		rts.push_back(new Route(p[i], t, f[i], c[i]));
	}
	return new Solution(rts);
}
Solution *GreedyInit::NewSol(){
	//puts("NewSol in");
	using pdata::dt;
	std::vector<std::pair<std::pair<double, double>, int> > v1;
	pdata::CalcJDis();
	for (int i = 1; i <= dt.N; ++i){
		double k = atan2(dt.vertex[dt.z[i]].y - dt.vertex[dt.Z].y, dt.vertex[dt.z[i]].x - dt.vertex[dt.Z].x);
		v1.push_back(std::make_pair(std::make_pair(dt.s[i], k), i));
	}
	//for (auto i: v1) printf("cus = %d\n", 
	std::sort(v1.begin(), v1.end());
	if (randomize)
		for (int i = 0; i < rand_times; ++i)
			std::swap(v1[pdata::rand(0, v1.size() - 1)], v1[pdata::rand(0, v1.size() - 1)]);
	return NewSolW(v1);
}

