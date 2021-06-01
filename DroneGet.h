//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#pragma once
#include "BaseSolC.h"
class DroneGet1 : public BaseDroneGet {
public:
	std::vector<Solution*> Get(Solution *);
protected:
	void ReplaceRoute(Route *&, Route *);
	Route * GetRouteInit(Route *);
	Route * GetRouteAdj(Route *);
	Route * GetRoute(Route * r, double Trate, double T0, int iters);

};
//Determine t, c, f

