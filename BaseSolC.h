//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#pragma once
#include "Route.h"
#include <random>
#include <initializer_list>	
//Get the adjacent solution
class BaseAdjGet{
public:
	virtual std::vector<Solution*> Get(Solution *) = 0;
} ;
class BaseDroneGet{
public:
	virtual std::vector<Solution*> Get(Solution *) = 0;
} ;
class BaseInit{
public:
	virtual Solution *NewSol() = 0;
} ;
class BaseSolSet{
protected:
	std::vector<BaseInit*> inits;
	int nIter;
	Solution *best;
public:
	BaseSolSet();
	BaseSolSet(std::initializer_list<BaseInit*>);
	virtual ~BaseSolSet();
	virtual BaseSolSet *Iteration(BaseAdjGet *, BaseDroneGet *) = 0;
	Solution *gbest();
} ;

