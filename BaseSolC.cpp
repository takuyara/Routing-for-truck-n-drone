//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#include "BaseSolC.h"
BaseSolSet::BaseSolSet(std::initializer_list<BaseInit*> p): nIter(0){
	inits.clear();
	for (auto i: p){
		inits.push_back(i);
		//printf("i Addr = %x\n", i);
		i->NewSol();
	}
}
BaseSolSet::BaseSolSet(){
	inits.clear();
}
BaseSolSet::~BaseSolSet(){
	for (auto i: inits) delete i;
}
Solution *BaseSolSet::gbest(){ return best; }

