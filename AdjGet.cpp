//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#include "AdjGet.h"
#include "pdata.h"
#include <vector>
#include <algorithm>

using pdata::dt;
using pdata::rand;
using pdata::rand01;

std::vector<Solution *> AdjGet1::Get(Solution * s) {
	std::vector<Solution *> ss({
		this->ApplyRouteAdd(s),
		this->ApplyRouteDel(s),
		this->ApplyRouteCross(s),
		this->ApplyRouteReverse(s),
		this->ApplyRouteNodeMove(s),
		this->ApplyRouteNodeExchange(s),
		this->ApplyRouteNodeInsert(s),
		this->ApplyRouteNodeRemove(s),
		this->ApplyCustomerMove(s),
		this->ApplyCustomerExchange(s)
	}), st;
	int sn = int(ss.size());
	st.reserve(sn);
	for (int i = 0; i < sn; ++i)
		if (ss[i])
			st.push_back(ss[i]);
	if (st.empty())
		st.push_back(new Solution(*s));
	return st;
}

Solution * AdjGet1::ApplyRouteAdd(Solution * s) {
	const std::vector<Route *> & s_rts = s->grts();
	int rn = int(s_rts.size());
	if (rn >= pdata::dt.M)
		return nullptr;
	std::vector<Route *> rts(rn);
	for (int i = 0; i < rn; ++i)
		rts[i] = new Route(*s_rts[i]);
	int src = pdata::dt.Z;
	std::vector<int> xs;
	for (int ei = pdata::e[src]; ~ei; ei = pdata::d[ei].n)
		xs.push_back(pdata::d[ei].y);
	rts.push_back(new Route(std::vector<int>({src, xs[rand(0, int(xs.size() - 1))], src}), std::vector<int>(), std::vector<int>(), std::vector<int>()));
	return new Solution(rts);
}

Solution * AdjGet1::ApplyRouteDel(Solution * s) {
	const std::vector<Route *> & s_rts = s->grts();
	int rn = int(s_rts.size());
	if (rn < 2)
		return nullptr;
	int ri, rj;
	do
		ri = rand(0, rn - 1), rj = rand(0, rn - 1);
	while (ri == rj);
	std::vector<Route *> rts;
	rts.reserve(rn - 1);
	for (int i = 0; i < rn; ++i)
		if (i == ri) {
			std::vector<int> ci(s_rts[ri]->gc());
			const std::vector<int> & cj = s_rts[rj]->gc();
			int cnj = int(cj.size());
			for (int j = 0; j < cnj; ++j)
				ci.push_back(cj[j]);
			rts.push_back(new Route(s_rts[i]->gp(), s_rts[i]->gt(), s_rts[i]->gf(), ci));
		}
		else if (i != rj)
			rts.push_back(new Route(*s_rts[i]));
	return new Solution(rts);
}

Solution * AdjGet1::ApplyRouteCross(Solution * s) {
	const std::vector<Route *> & s_rts = s->grts();
	int rn = int(s_rts.size());
	if (rn < 2)
		return nullptr;
	else {
		std::vector<Route *> rts(rn);
		int ri, rj;
		do
			ri = rand(0, rn - 1), rj = rand(0, rn - 1);
		while (ri == rj);
		const std::vector<int> & spi = s_rts[ri]->gp(), & spj = s_rts[rj]->gp();
		int pni = spi.size(), pnj = spj.size();
		std::map<int, int> jdx;
		for (int j = 1; j < pnj; ++j)
			if (jdx.find(spj[j]) == jdx.end() || rand01() < 0.5)
				jdx[spj[j]] = j;
		std::vector<std::pair<int, int>> candidates;
		for (int ip = 0; ip < pni - 1; ++ip)
			for (int ei = pdata::e[spi[ip]]; ~ei; ei = pdata::d[ei].n) {
				auto jit = jdx.find(pdata::d[ei].y);
				if (jit != jdx.end()) {
					int i = ip + 1, j = jit->second;
					if (pdata::dt.edge.find(std::make_pair(spi[i - 1], spj[j])) != pdata::dt.edge.end() && pdata::dt.edge.find(std::make_pair(spj[j - 1], spi[i])) != pdata::dt.edge.end())
						candidates.emplace_back(i, j);
				}
			}
		if (candidates.empty())
			return nullptr;
		int cand_i = rand(0, candidates.size() - 1);
		int ci = candidates[cand_i].first, cj = candidates[cand_i].second;
		std::vector<int> pi(spi.begin(), spi.begin() + ci), pj(spj.begin(), spj.begin() + cj);
		pi.reserve(pnj);
		for (int j = cj; j < pnj; ++j)
			pi.push_back(spj[j]);
		pj.reserve(pni);
		for (int i = ci; i < pni; ++i)
			pj.push_back(spi[i]);
		for (int i = 0; i < rn; ++i)
			if (i == ri)
				rts[i] = new Route(pi, s_rts[i]->gt(), s_rts[i]->gf(), s_rts[i]->gc());
			else if (i == rj)
				rts[i] = new Route(pj, s_rts[i]->gt(), s_rts[i]->gf(), s_rts[i]->gc());
			else
				rts[i] = new Route(*s_rts[i]);
		return new Solution(rts);
	}
}

Solution * AdjGet1::ApplyRouteReverse(Solution * s) {
	const std::vector<Route *> & s_rts = s->grts();
	int rn = int(s_rts.size());
	if (rn < 1)
		return nullptr;
	else {
		std::vector<Route *> rts(rn);
		int ri = rand(0, rn - 1);
		const std::vector<int> & sp = s_rts[ri]->gp();
		int pn = sp.size();
		std::map<int, int> jdx;
		for (int i = 1; i < pn; ++i)
			if (jdx.find(sp[i]) == jdx.end() || rand01() < 0.5)
				jdx[sp[i]] = i;
		std::vector<std::pair<int, int>> candidates;
		for (int ip = 0; ip < pn - 1; ++ip)
			for (int ei = pdata::e[sp[ip]]; ~ei; ei = pdata::d[ei].n) {
				auto jit = jdx.find(pdata::d[ei].y);
				if (jit != jdx.end()) {
					int i = ip + 1, j = jit->second;
					if (j > i + 1 && pdata::dt.edge.find(std::make_pair(sp[i - 1], sp[j - 1])) != pdata::dt.edge.end() && pdata::dt.edge.find(std::make_pair(sp[i], sp[j])) != pdata::dt.edge.end())
						candidates.emplace_back(i, j);
				}
			}
		if (candidates.empty())
			return nullptr;
		int cand_i = rand(0, candidates.size() - 1);
		int ci = candidates[cand_i].first, cj = candidates[cand_i].second;
		for (int i = 0; i < rn; ++i)
			if (i == ri) {
				std::vector<int> p(sp);
				std::reverse(p.begin() + ci, p.begin() + cj);
				rts[i] = new Route(p, s_rts[i]->gt(), s_rts[i]->gf(), s_rts[i]->gc());
			}
			else
				rts[i] = new Route(*s_rts[i]);
		return new Solution(rts);
	}
}

Solution * AdjGet1::ApplyRouteNodeMove(Solution * s) {
	const std::vector<Route *> & s_rts = s->grts();
	int rn = int(s_rts.size());
	if (rn < 2)
		return nullptr;
	else {
		std::vector<Route *> rts(rn);
		int ri, rj;
		do
			ri = rand(0, rn - 1), rj = rand(0, rn - 1);
		while (ri == rj);
		const std::vector<int> & spi = s_rts[ri]->gp(), & spj = s_rts[rj]->gp();
		int pni = spi.size(), pnj = spj.size();
		std::map<int, int> jdx;
		for (int j = 1; j < pnj - 1; ++j)
			if (jdx.find(spj[j]) == jdx.end() || rand01() < 0.5)
				jdx[spj[j]] = j;
		std::vector<std::pair<int, int>> candidates;
		for (int i = 0; i < pni - 1; ++i) {
			for (int ei = pdata::e[spi[i]]; ~ei; ei = pdata::d[ei].n) {
				std::map<int, int>::iterator jit = jdx.find(pdata::d[ei].y);
				if (jit != jdx.end()) {
					int j = jit->second;
					if (pdata::dt.edge.find(std::make_pair(spi[i], spj[j])) != pdata::dt.edge.end() && pdata::dt.edge.find(std::make_pair(spj[j], spi[i + 1])) != pdata::dt.edge.end() && pdata::dt.edge.find(std::make_pair(spj[j - 1], spj[j + 1])) != pdata::dt.edge.end())
						candidates.emplace_back(i, j);
				}
			}
		}
		if (candidates.empty())
			return nullptr;
		int cand_i = rand(0, candidates.size() - 1);
		for (int i = 0; i < rn; ++i)
			if (i == ri) {
				std::vector<int> p(spi);
				p.insert(p.begin() + candidates[cand_i].first + 1, spj[candidates[cand_i].second]);
				rts[i] = new Route(p, s_rts[i]->gt(), s_rts[i]->gf(), s_rts[i]->gc());
			}
			else if (i == rj) {
				std::vector<int> p(spj);
				p.erase(p.begin() + candidates[cand_i].second);
				rts[i] = new Route(p, s_rts[i]->gt(), s_rts[i]->gf(), s_rts[i]->gc());
			}
			else
				rts[i] = new Route(*s_rts[i]);
		return new Solution(rts);
	}
}

Solution * AdjGet1::ApplyRouteNodeExchange(Solution * s) {
	const std::vector<Route *> & s_rts = s->grts();
	int rn = int(s_rts.size());
	if (rn < 1)
		return nullptr;
	else {
		std::vector<Route *> rts(rn);
		int ri = rand(0, rn - 1), rj = rand(0, rn - 1);
		const std::vector<int> & spi = s_rts[ri]->gp(), & spj = s_rts[rj]->gp();
		int pni = spi.size(), pnj = spj.size();
		std::map<int, int> jdx;
		for (int j = 1; j < pnj - 1; ++j)
			if (jdx.find(spj[j]) == jdx.end() || rand01() < 0.5)
				jdx[spj[j]] = j;
		std::vector<std::pair<int, int>> candidates;
		for (int ip = 0; ip < pni - 2; ++ip) {
			for (int ei = pdata::e[spi[ip]]; ~ei; ei = pdata::d[ei].n) {
				auto jit = jdx.find(pdata::d[ei].y);
				if (jit != jdx.end()) {
					int i = ip + 1, j = jit->second;
					if (ri == rj) {
						if ((j == i + 1 && pdata::dt.edge.find(std::make_pair(spi[i - 1], spi[i + 1])) != pdata::dt.edge.end() && pdata::dt.edge.find(std::make_pair(spi[i], spi[i + 2])) != pdata::dt.edge.end()) || (j > i + 1 && pdata::dt.edge.find(std::make_pair(spi[i - 1], spj[j])) != pdata::dt.edge.end() && pdata::dt.edge.find(std::make_pair(spj[j], spi[i + 1])) != pdata::dt.edge.end() && pdata::dt.edge.find(std::make_pair(spj[j - 1], spi[i])) != pdata::dt.edge.end() && pdata::dt.edge.find(std::make_pair(spi[i], spj[j + 1])) != pdata::dt.edge.end()))
							candidates.emplace_back(i, j);
					}
					else { // ri != rj
						if (pdata::dt.edge.find(std::make_pair(spi[i - 1], spj[j])) != pdata::dt.edge.end() && pdata::dt.edge.find(std::make_pair(spj[j], spi[i + 1])) != pdata::dt.edge.end() && pdata::dt.edge.find(std::make_pair(spj[j - 1], spi[i])) != pdata::dt.edge.end() && pdata::dt.edge.find(std::make_pair(spi[i], spj[j + 1])) != pdata::dt.edge.end())
							candidates.emplace_back(i, j);
					}
				}
			}
		}
		if (candidates.empty())
			return nullptr;
		int cand_i = rand(0, candidates.size() - 1);
		for (int i = 0; i < rn; ++i)
			if (i == ri || i == rj) {
				std::vector<int> p(s_rts[i]->gp());
				if (ri == rj)
					std::swap(p[candidates[cand_i].first], p[candidates[cand_i].second]);
				else if (i == ri)
					p[candidates[cand_i].first] = s_rts[rj]->gp()[candidates[cand_i].second];
				else // j == rj
					p[candidates[cand_i].second] = s_rts[ri]->gp()[candidates[cand_i].first];
				rts[i] = new Route(p, s_rts[i]->gt(), s_rts[i]->gf(), s_rts[i]->gc());
			}
			else
				rts[i] = new Route(*s_rts[i]);
		return new Solution(rts);
	}
}

Solution * AdjGet1::ApplyRouteNodeInsert(Solution * s) {
	const std::vector<Route *> & s_rts = s->grts();
	int rn = int(s_rts.size());
	if (rn < 1)
		return nullptr;
	else {
		std::vector<Route *> rts(rn);
		int ri = rand(0, rn - 1);
		const std::vector<int> & sp = s_rts[ri]->gp();
		int pn = sp.size();
		std::vector<std::pair<int, int>> candidates;
		for (int i = 0; i < pn - 1; ++i)
			for (int ek = pdata::e[sp[i]]; ~ek; ek = pdata::d[ek].n)
				if (dt.edge.find(std::make_pair(pdata::d[ek].y, sp[i + 1])) != dt.edge.end())
					candidates.emplace_back(i + 1, pdata::d[ek].y);
		if (candidates.empty())
			return nullptr;
		int cand_i = rand(0, candidates.size() - 1);
		for (int i = 0; i < rn; ++i)
			if (i == ri) {
				std::vector<int> p(sp);
				p.insert(p.begin() + candidates[cand_i].first, candidates[cand_i].second);
				rts[i] = new Route(p, s_rts[ri]->gt(), s_rts[ri]->gf(), s_rts[ri]->gc());
			}
			else
				rts[i] = new Route(*s_rts[i]);
		return new Solution(rts);
	}
}

Solution * AdjGet1::ApplyRouteNodeRemove(Solution * s) {
	const std::vector<Route *> & s_rts = s->grts();
	int rn = int(s_rts.size());
	if (rn < 1)
		return nullptr;
	else {
		std::vector<Route *> rts(rn);
		int ri = rand(0, rn - 1);
		const std::vector<int> & sp = s_rts[ri]->gp();
		int pn = sp.size();
		std::vector<int> candidates;
		for (int i = 1; i < pn - 1; ++i)
			if (dt.edge.find(std::make_pair(sp[i - 1], sp[i + 1])) != dt.edge.end())
				candidates.push_back(i);
		if (candidates.empty())
			return nullptr;
		int pi = candidates[rand(0, candidates.size() - 1)];
		for (int i = 0; i < rn; ++i)
			if (i == ri) {
				std::vector<int> p(sp);
				p.erase(p.begin() + pi);
				rts[i] = new Route(p, s_rts[ri]->gt(), s_rts[ri]->gf(), s_rts[ri]->gc());
			}
			else
				rts[i] = new Route(*s_rts[i]);
		return new Solution(rts);
	}
}

Solution * AdjGet1::ApplyCustomerMove(Solution * s) {
	const std::vector<Route *> & s_rts = s->grts();
	int rn = int(s_rts.size());
	if (rn < 2)
		return nullptr;
	else {
		std::vector<Route *> rts(rn);
		int ri, rj;
		do
			ri = pdata::rand(0, rn - 1), rj = pdata::rand(0, rn - 1);
		while (ri == rj);
		int rim = s_rts[ri]->gc().size();
		if (rim < 1)
			return nullptr;
		int ci = rand(0, rim - 1);
		for (int i = 0; i < rn; ++i)
			if (i == ri) {
				std::vector<int> c(s_rts[ri]->gc());
				c.erase(c.begin() + ci);
				rts[i] = new Route(s_rts[ri]->gp(), s_rts[ri]->gt(), s_rts[ri]->gf(), c);
			}
			else if (i == rj) {
				std::vector<int> c(s_rts[rj]->gc());
				c.push_back(s_rts[ri]->gc()[ci]);
				rts[i] = new Route(s_rts[rj]->gp(), s_rts[rj]->gt(), s_rts[rj]->gf(), c);
			}
			else
				rts[i] = new Route(*s_rts[i]);
		return new Solution(rts);
	}
}

Solution * AdjGet1::ApplyCustomerExchange(Solution * s) {
	const std::vector<Route *> & s_rts = s->grts();
	int rn = int(s_rts.size());
	if (rn < 2)
		return nullptr;
	else {
		std::vector<Route *> rts(rn);
		int ri, rj;
		do
			ri = pdata::rand(0, rn - 1), rj = pdata::rand(0, rn - 1);
		while (ri == rj);
		for (int i = 0; i < rn; ++i)
			if (i == ri)
				rts[i] = new Route(s_rts[ri]->gp(), s_rts[ri]->gt(), s_rts[ri]->gf(), s_rts[rj]->gc());
			else if (i == rj)
				rts[i] = new Route(s_rts[rj]->gp(), s_rts[rj]->gt(), s_rts[rj]->gf(), s_rts[ri]->gc());
			else
				rts[i] = new Route(*s_rts[i]);
		return new Solution(rts);
	}
}
