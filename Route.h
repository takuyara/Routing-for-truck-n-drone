//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#pragma once
#include "utils.h"
#include "BaseData.h"
//Derive classes from these like GARoute, TBSRoute.
class Route{
protected:
	//p: Every vertex the truck comes to in order.
	vec p, t, f, c;
	double rettime;
public:
	Route();
	Route(const vec &, const vec &, const vec &, const vec &);
	Route(const Route &);
	virtual ~Route();
	//Returns tuple: p, t, f, c.
	const vec &gp() const;
	const vec &gt() const;
	const vec &gf() const;
	const vec &gc() const;
	virtual void ShowResult(BaseData *) const;
	double ActualCost(BaseData *) const;
	//Compute.first == cost, Compute.second == return time.
	paDouble Compute(BaseData *) const;
	double CostWithoutCheck(BaseData *) const;
	virtual double PenalizedCost(BaseData *) const;
	virtual void AssertValid(BaseData *) const;
} ;

//Also Derive it with GASolution if in need.
class Solution{
protected:
	std::vector<Route*> rts;
public:
	Solution();
	Solution(const Solution &);
	Solution(const std::vector<Route*> &);
	virtual ~Solution();
	const std::vector<Route*> &grts();
	//Check if all the customers are served.
	//Assert valid for all routes.
	virtual void AssertValid(BaseData *);
	double ActualCost(BaseData *);
	double CostWithoutCheck(BaseData *);
	virtual double PenalizedCost(BaseData *);
	virtual void ShowResult(BaseData *);
	void ReadFromFile(std::string);
	void SaveToFile(std::string);
} ;

/*
class GASolution: public Solution{
public:
	double PenalizedCost(BaseData *d){
		try{
			AssertValid(d);
		} catch (TooManyGoodsException &e){
			Penalty = 1000;
			Cost = 
			e.what()
		} catch (MyException &e){
			puts(e.what());
		}
	}
} ;
*/

