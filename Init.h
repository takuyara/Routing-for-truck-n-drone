//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#pragma once
#include "BaseSolC.h"
class GreedyInit: public BaseInit{
	bool randomize;
public:
	GreedyInit(bool f = true);
	Solution *NewSol() override;
	Solution *NewSolW(const std::vector<std::pair<std::pair<double, double>, int> > &);
} ;

