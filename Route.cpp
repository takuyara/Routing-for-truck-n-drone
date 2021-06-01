//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#include "Route.h"
#include "Exceptions.h"
#include "config.h"
#include "FastReader.h"
#include <cmath>
#include <cstring>
#include <algorithm>
using std::get;
Route::Route(): p(){ }
Route::Route(const vec &p1, const vec &t1, const vec &f1, const vec &c1):
	p(p1), t(t1), f(f1), c(c1){ }
Route::~Route(){
	p.clear(), t.clear(), f.clear(), c.clear();
}
Route::Route(const Route &a): p(a.p), t(a.t), f(a.f), c(a.c){
}
const vec &Route::gp() const{ return p; }
const vec &Route::gt() const{ return t; }
const vec &Route::gf() const{ return f; }
const vec &Route::gc() const{ return c; }
void Route::AssertValid(BaseData *d) const{
	int sz = p.size(), cnt = 0;
	if (sz < 2) throw InvalidVertexException();
	if (p[0] != d->Z || p[sz - 1] != d->Z) throw NotTourException();
	for (int i = 0; i < sz - 1; ++i){
		if (p[i] < 0 || p[i] > d->V) throw InvalidVertexException();
		if (d->edge.find(std::make_pair(p[i], p[i + 1])) == d->edge.end())
			throw NoSuchEdgeException();
		if (p[i] == d->Z) ++cnt;
	}
	if (cnt != t.size()) throw LengthErrorException();
	if (f.size() != c.size()) throw LengthErrorException();
	for (int i = 0; i < f.size(); ++i){
		if (i != f.size() - 1 && f[i] > f[i + 1]) throw NotInOrderException();
		if (c[i] < 1 || c[i] > d->N) throw InvalidCustomerException();
		if (f[i] > sz - 1) throw InvalidIndexException();
	}
	for (int i = 1, j = 0, c1; i < sz; ++i){
		if (p[i] == d->V){
			for (c1 = 0; j < f.size() && f[j] <= i; ++j) ++c1;
			if (c1 > d->L) throw TooManyGoodsException();
		}
	}
}
paDouble Route::Compute(BaseData *d) const{
	double curtime = 0, cost = 0;
	int pot = 0, po1 = 0, u, v;
	//printf("psz = %d\n", p.size());
	for (int i = 0, sz = p.size(); i < sz - 1; ++i){
		u = p[i], v = p[i + 1];
		paDouble res = d->edge[std::make_pair(u, v)];
		if (u == d->Z) curtime += t[pot++];
		for (; po1 < f.size() && f[po1] == i; ++po1){
			int cust = c[po1];
			double flyds = dist(d->vertex[u], d->vertex[d->z[cust]]);
			curtime += d->R * flyds;
			//cost += d->P * (std::min(d->t[cust], curtime) - std::max(d->s[cust], curtime)) + d->S * flyds;
			//double tt = d->P * (std::min(d->t[cust], curtime) - std::max(d->s[cust], curtime));
			//if (tt < 0) puts("Fuck we have a problem on tt");
			//if (d->t[cust] < d->s[cust]) puts("Problem on time window");
			//if (cust < 0 || cust > d->N) puts("Wrong customer index");
			double timePenalty = 0;
			if (curtime > d->t[cust]) timePenalty = curtime - d->t[cust];
			if (curtime < d->s[cust]) timePenalty = d->s[cust] - curtime;
			cost += timePenalty * d->P + d->S * flyds;
			curtime += d->R * flyds;
		}
		curtime += res.second;
		cost += res.first;
		//if (res.first < 0) puts("Problem on res");
	}
	//printf("Return time = %.2lf\n", curtime);
	return std::make_pair(cost, curtime);
}
double Route::CostWithoutCheck(BaseData *d) const{
	return Compute(d).first;
}
double Route::ActualCost(BaseData *d) const{
	//try{
		Route::AssertValid(d);
		return CostWithoutCheck(d);
	//} catch (MyException &e){
	//	puts(e.what());
	//	return 1e100;
	//}
}
double Route::PenalizedCost(BaseData *d) const{
	return ActualCost(d);
}
void Route::ShowResult(BaseData *d) const{
	printf("Cost of this route: %.2lf\n", ActualCost(d));
}
Solution::Solution(){ }
Solution::Solution(const Solution &p){
	rts.clear();
	for (auto i: p.rts) rts.push_back(new Route(*i));
}
Solution::Solution(const std::vector<Route*> &p): rts(p){
}
Solution::~Solution(){
	for (auto p: rts) delete p;
	rts.clear();
}
const std::vector<Route*> &Solution::grts(){ return rts; }
void Solution::AssertValid(BaseData *d){
	static bool vis[maxn];
	memset(vis, 0, sizeof(vis));
	if (rts.size() > d->M) throw TooManyTrucksException(); 
	for (auto p: rts){
		auto c = p->gc();
		for (auto i: c) vis[i] = true;
		p->AssertValid(d);
	}
	for (int i = 1; i <= d->N; ++i)
		if (!vis[i]) throw CustomerNotServedException();
}
double Solution::ActualCost(BaseData *d){
	//try{
		Solution::AssertValid(d);
		return CostWithoutCheck(d);
	//} catch (MyException &e){
	//	puts(e.what());
	//	return 1e100;
	//}
}
double Solution::CostWithoutCheck(BaseData *d){
	double cost = 0;
	for (auto p: rts) cost += p->ActualCost(d);
	return cost;
}
double Solution::PenalizedCost(BaseData *d){
	return ActualCost(d);
}
void Solution::ShowResult(BaseData *d){
	try{
		AssertValid(d);
	} catch (MyException &e){
		puts(e.what());
		puts("This solution is invalid.");
		return ;
	}
	double cost = 0; int i = 0;
	for (auto p: rts){
		double tmp = p->ActualCost(d);
		printf("Route: %d, cost = %.2lf\n", ++i, tmp);
		cost += tmp;
	}
	printf("Total cost = %.2f\n", cost);
}
void Solution::ReadFromFile(std::string filename){
	try {
		FastReader frd(filename);
		int n = frd.readint();
		//fprintf(stderr, "%d\n", n);
		rts.clear();
		for (int i = 0; i < n; ++i){
			vec p, t, f, c;
			int pl = frd.readint();
			while (pl--) p.push_back(frd.readint());
			int tl = frd.readint();
			while (tl--) t.push_back(frd.readint());
			int fl = frd.readint();
			while (fl--) f.push_back(frd.readint());
			int cl = frd.readint();
			while (cl--) c.push_back(frd.readint());
			rts.push_back(new Route(p, t, f, c));
		}
	} catch (MyException &e){
		puts(e.what());
	}
}
void Solution::SaveToFile(std::string filename){
	FILE *fp = fopen(filename.c_str(), "w");
	fprintf(fp, "%d\n", rts.size());
	for (auto i: rts){
		fprintf(fp, "%d\n", i->gp().size());
		for (auto j: i->gp()) fprintf(fp, "%d ", j);
		fprintf(fp, "\n%d\n", i->gt().size());
		for (auto j: i->gt()) fprintf(fp, "%d ", j);
		fprintf(fp, "\n%d\n", i->gf().size());
		for (auto j: i->gf()) fprintf(fp, "%d ", j);
		fprintf(fp, "\n%d\n", i->gc().size());
		for (auto j: i->gc()) fprintf(fp, "%d ", j);
		fprintf(fp, "\n");
	}
	fclose(fp);
}

