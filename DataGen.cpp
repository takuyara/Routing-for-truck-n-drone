//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#include "BaseData.h"
#include "gconfig.h"
#include "utils.h"
#include <random>
#include <vector>
#include <algorithm>
std::mt19937 generator(2136);
//Generate a random number from (-b, -a) to (a, b)
//Distribution: Cauchy(0, 1). The parts where x < -2.5 and x > 2.5 are aborted.
//bool Gen;
inline double randcau(double a, double b){
	//static std::random_device rand_device;
	static std::cauchy_distribution<double> caudis(0.0, 1.0);
	double x;
	while ((x = caudis(generator)) < -2.5 || x > 2.5) ;
	//printf("%.2lf\n", x);
	//if (x > 2.5 || x < -2.5) puts("Fuck");
	x *= ((b - a) / 2.5);
	//if (x > b || x < -b) puts("Fuck");
	x = x < 0 ? x - a : x + a;
	//if (x < 0) printf("Minus! %.2lf\n", x);
	//if (Gen) printf("%.2lf %.2lf %.2lf\n", x, a, b);
	return x;
}
//Generate uniformly distributed random number.
inline double randuni(int x = 0, int y = 1){
	static std::uniform_real_distribution<double> unidis(0.0, 1.0);
	return unidis(generator) * (y - x) + x;
}
inline int randuni_int(int x, int y){
	static std::uniform_int_distribution<int> unidis(0, gconfig::maxint);
	return unidis(generator) % (y - x) + x;
}
bool iscity[maxv];
int fa[maxv];
BaseData dt;
std::vector<int> conset[maxv], rtvec;
int getrt(int x){
	/*
	if (x == 9911){
		while (~fa[x]){
			printf("fa = %d\n", fa[x]), x = fa[x];
		}
	}
	while (~fa[x]) x = fa[x];
	return x;
	*/
	return ~fa[x] ? (fa[x] = getrt(fa[x])) : x;
}
int main(){
	//Generate Vertices
	int highwaycnt = 0;
	dt.V = gconfig::V, dt.Z = gconfig::Z;
	for (int i = 1; i <= dt.V; ++i){
		//Determine whether it's in city or suburb.
		if (i == dt.Z || randuni() < gconfig::city_rate){
			iscity[i] = true;
			//Gen = true;
			dt.vertex[i] = Point(randcau(0, gconfig::cbonder), randcau(0, gconfig::cbonder));
			//Gen = false;
			//printf("City: %.2lf %.2lf\n", dt.vertex[i].x, dt.vertex[i].y);
		} else {
			iscity[i] = false;
			if (randuni() < 0.5)
				dt.vertex[i] = Point(randcau(gconfig::cbonder, gconfig::bonder), randcau(0, gconfig::bonder));
			else dt.vertex[i] = Point(randcau(0, gconfig::bonder), randcau(gconfig::cbonder, gconfig::bonder));	
		}
	}
	//Generate Edges
	static std::vector<std::pair<double, double> > mean_s, mean_h;
	mean_s.clear(), mean_h.clear();
	for (int i = 1; i <= dt.V; ++i){
		int nhighway, nstreet;
		if (iscity[i]){
			nhighway = randuni() > gconfig::highway_prob ? 0 : 1;
			nstreet = randuni(3, 5);
		} else {
			nhighway = 0;
			nstreet = randuni(0, 3);
		}
		static std::vector<std::pair<double, int> > vec, vec_s, vec_h;
		vec.clear(), vec_s.clear(), vec_h.clear();
		for (int j = 1; j <= dt.V; ++j)
			if (i != j) vec.push_back(std::make_pair(dist(dt.vertex[i], dt.vertex[j]), j));
		std::sort(vec.begin(), vec.end());
		for (int k = 0, j = vec.size() - gconfig::crossprev - 1;
			k < nhighway * gconfig::highway_rate; ++k, --j){
			while (!iscity[vec[j].second]) --j;
			vec_h.push_back(vec[j]);
		}
		for (int j = 0; j < nstreet * gconfig::street_rate; ++j) vec_s.push_back(vec[j]);
		std::random_shuffle(vec_h.begin(), vec_h.end());
		std::random_shuffle(vec_s.begin(), vec_s.end());
		int ns = 0, nh = 0;
		if (iscity[i]){
			for (auto p: vec_h){
				int j = p.second;
				if (dt.edge.find(std::make_pair(i, j)) != dt.edge.end()) continue;
				if (p.first > gconfig::max_highway_len) continue;
				double d1 = p.first * randuni(0.85, 1.15), d2 = p.first * randuni(0.85, 1.15);
				dt.edge[std::make_pair(i, j)]
					= std::make_pair(gconfig::highw * d1 + gconfig::highwb, gconfig::hight * d1);
				dt.edge[std::make_pair(j, i)]
					= std::make_pair(gconfig::highw * d2 + gconfig::highwb, gconfig::hight * d2);
				mean_h.push_back(dt.edge[std::make_pair(i, j)]);
				mean_h.push_back(dt.edge[std::make_pair(j, i)]);
				//if (j == 3951 || j == 4045 || j == 8537 || j == 9911){
				//	printf("NotG: %d\n", i);
				//	for (int k = 0; k < 10; ++k) printf("To: %d, Dis = %.2lf\n", vec_h[k].second, vec_h[k].first);
				//}
				highwaycnt += 2;
				if (++nh >= nhighway) break;
			}
		}
		for (auto p: vec_s){
			int j = p.second;
			if (dt.edge.find(std::make_pair(i, j)) != dt.edge.end()) continue;
			double d1 = p.first * randuni(0.85, 1.15), d2 = p.first * randuni(0.85, 1.15);
			dt.edge[std::make_pair(i, j)] = std::make_pair(gconfig::streetw * d1, gconfig::streett * d1);
			dt.edge[std::make_pair(j, i)] = std::make_pair(gconfig::streetw * d2, gconfig::streett * d2);
			mean_s.push_back(dt.edge[std::make_pair(i, j)]);
			mean_s.push_back(dt.edge[std::make_pair(j, i)]);
			if (++ns == nstreet) break;
		}
	}
	for (auto p: dt.edge){
		int x = p.first.first;
	}
	//Deal with unconnected components.
	for (int i = 1; i <= dt.V; ++i)
		fa[i] = -1, conset[i].clear();
	//puts("Get into this");
	rtvec.clear();
	for (auto p: dt.edge){
		//printf("%d %d, %d %d\n", p.first.first, p.first.second, getrt(p.first.first), getrt(p.first.second));
		int i = p.first.first, j = p.first.second;
		if (getrt(i) != getrt(j)) fa[getrt(i)] = getrt(j);
		//puts("Finish it");
	}
	/*
	for (auto p = dt.edge.begin(); p != dt.edge.end(); ++p){
		puts("Getin");
		int i = p->first.first, j = p->first.second;
		printf("%d %d, %d\n", i, j, getrt(i));
		printf("%d\n", getrt(j));
		fa[getrt(i)] = getrt(j);
		puts("Finish");
	}
	*/
	//puts("Deal finish");
	for (int i = 1; i <= dt.V; ++i){
		//puts("Get it");
		conset[getrt(i)].push_back(i);
		if (getrt(i) == i) rtvec.push_back(i);
		//printf("%d: %d\n", i, getrt(i));
	}
	//printf("Len = %d\n", rtvec.size());
	for (auto i: rtvec){
		int x1 = 0, y1 = 0;
		double mndis = 1e100, t;
		for (auto j: rtvec) if (i != j)
			for (auto x: conset[i])
				for (auto y: conset[j])
					if ((t = dist(dt.vertex[x], dt.vertex[y])) < mndis)
						if (getrt(x) != getrt(y)) mndis = t, x1 = x, y1 = y;
		//printf("%.2lf\n", mndis);
		if (mndis < gconfig::max_street_len){
			double d1 = mndis * randuni(0.85, 1.15), d2 = mndis * randuni(0.85, 1.15);
			dt.edge[std::make_pair(x1, y1)] = std::make_pair(gconfig::streetw * d1, gconfig::streett * d1);
			dt.edge[std::make_pair(y1, x1)] = std::make_pair(gconfig::streetw * d2, gconfig::streett * d2);
			mean_s.push_back(dt.edge[std::make_pair(x1, y1)]);
			mean_s.push_back(dt.edge[std::make_pair(y1, x1)]);
			fa[getrt(x1)] = getrt(y1);
			//printf("New Street: %d %d\n", x1, y1);
			//printf("Dis = %.2lf\n", mndis);
		}
	}
	//# Edges
	dt.E = dt.edge.size();
	//Some Constants
	dt.M = gconfig::M, dt.L = gconfig::L;
	dt.S = gconfig::S, dt.R = gconfig::R, dt.P = gconfig::P;
	//Generate Customers
	dt.N = gconfig::N;
	for (int i = 1, j; i <= dt.N; ++i){
		//12% in suburb.
		bool oncity = randuni() < gconfig::city_cust_rate;
		while (iscity[j = randuni_int(1, dt.V)] != oncity) ;
		dt.z[i] = j;
		if (oncity){
			if (randuni() < gconfig::regular_time_rate){
				//Time window on work hours
				dt.s[i] = randuni(gconfig::rst1, gconfig::rfn1 - gconfig::min_city_interval);
				dt.t[i] = dt.s[i] + randuni(gconfig::min_city_interval, gconfig::rfn1 - dt.s[i]);
			} else if (randuni() < gconfig::regular_time_rate){
				//Time window on rest hours
				dt.s[i] = randuni(gconfig::rst2, gconfig::rfn2 - gconfig::min_city_interval);
				dt.t[i] = dt.s[i] + randuni(gconfig::min_city_interval, gconfig::rfn2 - dt.s[i]);
			} else {
				//No special needs
				dt.s[i] = randuni(0, gconfig::max_time - gconfig::min_city_interval);
				dt.t[i] = dt.s[i] + randuni(gconfig::min_city_interval, gconfig::max_time - dt.s[i]);
			}
		} else {
			//For those in suburb, they don't have too much need in time window.
			dt.s[i] = randuni(0, gconfig::max_time - gconfig::suburb_interval);
			dt.t[i] = dt.s[i] + gconfig::suburb_interval;
		}
	}
	//Adjust costs
	for (auto &i: dt.edge) i.second.first /= 2.5;
	dt.S /= 2.5, dt.P /= 2.5;
	dt.SaveToFile(gconfig::file_name);
	//Data Review
	double h_w = 0, h_t = 0, s_w = 0, s_t = 0;
	for (auto i: mean_h) h_w += i.first, h_t += i.second;
	for (auto i: mean_s) s_w += i.first, s_t += i.second;
	h_w /= mean_h.size(), h_t /= mean_h.size();
	s_w /= mean_s.size(), s_t /= mean_s.size();
	printf("Average of Highway: Cost = %.2lf, Time = %.2lf\n", h_w, h_t);
	printf("Average of Street: Cost = %.2lf, Time = %.2lf\n", s_w, s_t);
	printf("Highway precentage: %d / %d\n", highwaycnt, dt.E);
	return 0;
}

