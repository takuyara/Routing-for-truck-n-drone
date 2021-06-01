//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#pragma once
#include "BaseSolC.h"
class AdjGet1: public BaseAdjGet{
public:
	std::vector<Solution*> Get(Solution *);
protected:
	Solution * ApplyRouteAdd(Solution *);
	Solution * ApplyRouteDel(Solution *);
	Solution * ApplyRouteCross(Solution *);
	Solution * ApplyRouteReverse(Solution *);
	Solution * ApplyRouteNodeMove(Solution *);
	Solution * ApplyRouteNodeExchange(Solution *);
	Solution * ApplyRouteNodeInsert(Solution *);
	Solution * ApplyRouteNodeRemove(Solution *);
	Solution * ApplyCustomerMove(Solution *);
	Solution * ApplyCustomerExchange(Solution *);
} ;
//Determine p, c_raw
//p route
//t rest time
//f flight node: p[f[i]]
//c customer index at ith flight: c[i], customer vertex: dt.z[c[i]]
//Constructor: Solution vector<Route*>
//Constructor: Route vector<int> * 4, p, t, f, c
// Solution: grts; Route: gp, gt, gf, gc
// dt.edge[std::make_pair(u, v)].first: cost, second: time
// d[p].y: v, d[p].n: next edge index, d[p].c: cost, d[p].t: time, e[u]: header index

