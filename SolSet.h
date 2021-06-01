//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#pragma once
#include "BaseSolC.h"
class S1SolSet: public BaseSolSet{
protected:
	Solution *cur;
public:
	S1SolSet(std::initializer_list<BaseInit *>);
	S1SolSet(Solution *, Solution *, int);
	virtual ~S1SolSet();
	BaseSolSet *Iteration(BaseAdjGet *, BaseDroneGet *);
} ;
class SnSolSet: public BaseSolSet{
protected:
	std::vector<Solution*> cur;
public:
	SnSolSet(std::initializer_list<BaseInit*>);
	SnSolSet(const std::vector<Solution*> &, Solution *, int);
	virtual ~SnSolSet();
	BaseSolSet *Iteration(BaseAdjGet *, BaseDroneGet *);
} ;

