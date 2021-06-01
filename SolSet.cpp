//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#include "SolSet.h"
#include "pdata.h"
#include "Exceptions.h"
#include <algorithm>
S1SolSet::S1SolSet(std::initializer_list<BaseInit*> p):
	BaseSolSet(p){
	std::vector<Solution*> ps;
	ps.clear();
	//printf("Init sz = %d\n", inits.size());
	for (int i = 1; i <= beam; ++i){
		int x = pdata::rand(0, inits.size() - 1);
		//printf("Rand = %d\n", x);
		BaseInit *ini = inits[x];
		ps.push_back(ini->NewSol());
	}
	double mncost = 1e100, t;
	for (auto i: ps){
		try{
			if (mncost > (t = i->ActualCost(&pdata::dt)))
				mncost = t, cur = i;
			//printf("Mncost = %.2lf\n", t);
		} catch (HardConstraintsException &e){
			puts("Error: Invalid initial solution!\n");
			puts(e.what());
		}
	}
	best = cur;
	//printf("Best cost = %.2lf\n", best->ActualCost(&pdata::dt));
}
S1SolSet::S1SolSet(Solution *cur1, Solution *best1, int x):
	BaseSolSet(){
	cur = new Solution(*cur1);
	best = new Solution(*best1);
	nIter = x;
}
S1SolSet::~S1SolSet(){
	delete best;
	delete cur;
}
BaseSolSet *S1SolSet::Iteration(BaseAdjGet *adj, BaseDroneGet *drone){
	//puts("Iteration IN");
	auto res = adj->Get(cur);
	//puts("Adj Get Finish");
	double curc = cur->ActualCost(&pdata::dt), bestc = best->ActualCost(&pdata::dt);
	//puts("Actual cost finish");
	for (auto i: res){
		auto res_d = drone->Get(i);
		//printf("Drone get finish");
		for (auto j: res_d){
			try{
				double t = j->ActualCost(&pdata::dt);
				//printf("Get Acutalcost : %.2lf\n", t);
				if (t < bestc){
					//puts("Before delete best");
					delete best;
					//puts("After delete best");
					best = new Solution(*j), bestc = t;
				}
				double prob = exp(-(t - curc) / config::SA_T(nIter));
				if (pdata::rand01() < prob){
					delete cur;
					curc = t, cur = new Solution(*j);
				}
			} catch (HardConstraintsException &e){
				puts("Error: Invalid solution while getting adjacent solution.");
				puts(e.what());
			}
		}
		//puts("Before clear resd");
		for (auto j: res_d) delete j;
		//puts("After clear resd");
	}
	//puts("Before res");
	for (auto i: res) delete i;
	//puts("After res");
	return new S1SolSet(cur, best, nIter + 1);
}
SnSolSet::SnSolSet(std::initializer_list<BaseInit*> p): BaseSolSet(p){
	cur.clear();
	for (int i = 1; i <= beam; ++i){
		BaseInit *ini = inits[pdata::rand(0, inits.size() - 1)];
		cur.push_back(ini->NewSol());
	}
	double mncost = 1e100, t;
	for (auto i: cur){
		try{
			if (mncost > (t = i->ActualCost(&pdata::dt)))
				mncost = t, best = i;
		} catch (HardConstraintsException &e){
			puts("Error: Invalid initial solution!\n");
			puts(e.what());
		}
	}
	// TODO: std::sort(cur.begin(), cur.end());
	int cnt = cur.size() - beam;
	while (cnt--) cur.pop_back();
}
SnSolSet::SnSolSet(const std::vector<Solution*> &cur1, Solution *best1, int x):
	BaseSolSet(){
	cur.clear();
	for (auto i: cur1) cur.push_back(new Solution(*i));
	best = new Solution(*best1);
	nIter = x;
}
SnSolSet::~SnSolSet(){
	delete best;
	for (auto i: cur) delete i;
}
BaseSolSet *SnSolSet::Iteration(BaseAdjGet *adj, BaseDroneGet *drone){
	std::vector<std::pair<double, Solution*> > allpop;
	allpop.clear();
	for (auto l: cur){
		auto res = adj->Get(l);
		double curc = l->ActualCost(&pdata::dt), bestc = best->ActualCost(&pdata::dt);
		for (auto i: res){
			auto res_d = drone->Get(i);
			for (auto j: res_d){
				try{
					double t = j->ActualCost(&pdata::dt);
					if (t < bestc) best = j, bestc = t;
					double prob = exp(-(t - curc) / config::SA_T(nIter));
					if (pdata::rand01() < prob) allpop.push_back(std::make_pair(t, j));
				} catch (HardConstraintsException &e){
					puts("Error: Invalid solution while getting adjacent solution.");
					puts(e.what());
				}
			}
		}
	}
	std::sort(allpop.begin(), allpop.end());
	cur.clear();
	int cnt = 0;
	for (auto i: allpop){
		cur.push_back(i.second);
		if (++cnt == beam) break;
	}
	return new SnSolSet(cur, best, nIter + 1);
}

