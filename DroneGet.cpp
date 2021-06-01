//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#include "DroneGet.h"
#include "pdata.h"
#include "utils.h"
#include <algorithm>
using namespace pdata;
void DroneGet1::ReplaceRoute(Route *& cur, Route * tmp) {
	delete cur; cur = tmp;
}
Route * DroneGet1::GetRouteInit(Route * r) {
	int src = pdata::dt.Z;
	static const double PHI = (std::sqrt(5.) - 1.) / 2.;
	const std::vector<int> & rp = r->gp(), & rc = r->gc();
	int rpn = rp.size(), rcn = rc.size();
	std::vector<int> t, q(rcn), k(rcn), f(rcn), c(rcn);
	std::vector<double> d(rcn);
	for (int ci = 0; ci < rcn; ++ci)
		d[ci] = dist(dt.vertex[rp[q[ci] = 0]], dt.vertex[rc[ci]]);
	for (int pi = 0; pi < rpn - 1; ++pi) {
		if (rp[pi] == src)
			t.push_back(0);
		for (int ci = 0; ci < rcn; ++ci) {
			double dis = dist(dt.vertex[rp[pi]], dt.vertex[rc[ci]]);
			if (d[ci] > dis)
				d[ci] = dis, q[ci] = pi;
		}
	}
	for (int ci = 0; ci < rcn; ++ci)
		k[ci] = ci;
	std::sort(k.begin(), k.end(), [q](int i, int j) -> bool { return q[i] < q[j]; });
	for (int i = 0; i < rcn; ++i)
		f[i] = q[k[i]], c[i] = rc[k[i]];
	return new Route(rp, t, f, c);
}
Route * DroneGet1::GetRouteAdj(Route * r) {
	const std::vector<int> & rp = r->gp(), & rf = r->gf(), & rc = r->gc();
	int tn = int(r->gt().size());
	std::vector<int> t(tn);
	for (int i = 0; i < tn; ++i)
		t[i] = rand(0, 300);
	return new Route(rp, t, rf, rc);
}
Route * DroneGet1::GetRoute(Route * r, double Trate = 0.97, double T0 = 1., int iters = 10) {
	Route * cur = this->GetRouteInit(r), * best = new Route(*cur), * tmp;
	double cur_ans = cur->ActualCost(&dt), best_ans = cur_ans, tmp_ans;
	int it; double T;
	for (it = 0, T = T0; it < iters; ++it, T *= Trate) {
		tmp = this->GetRouteAdj(cur);
		tmp_ans = tmp->ActualCost(&dt);
		if (rand01() <= std::exp((cur_ans - tmp_ans) / T)) {
			this->ReplaceRoute(cur, tmp);
			cur_ans = tmp_ans;
		} else {
			delete tmp;
		}
		if (cur_ans < best_ans) {
			this->ReplaceRoute(best, new Route(*cur));
			best_ans = cur_ans;
		}
	}
	delete cur;
	return best;
}
std::vector<Solution *> DroneGet1::Get(Solution * s) {
	const std::vector<Route *> & s_rts = s->grts();
	int rn = int(s_rts.size());
	std::vector<Route *> rts(rn);
	for (int ri = 0; ri < rn; ++ri)
		rts[ri] = this->GetRoute(s_rts[ri]);
	return std::vector<Solution *>({new Solution(rts)});
}
