//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#include "BaseData.h"
#include "Exceptions.h"
#include "FastReader.h"
void BaseData::SaveToFile(const std::string &fname){
	FILE *fPointer = fopen(fname.c_str(), "w");
	fprintf(fPointer, "%d %d %d %d %d %d\n", V, E, M, L, N, Z);
	fprintf(fPointer, "%.2lf %.2lf %.2lf\n", S, R, P);
	for (int i = 1; i <= V; ++i)
		fprintf(fPointer, "%.2lf %.2lf\n", vertex[i].x, vertex[i].y);
	for (auto i = edge.begin(); i != edge.end(); ++i){
		int u = i->first.first, v = i->first.second;
		double w = i->second.first, t = i->second.second;
		fprintf(fPointer, "%d %d %.2lf %.2lf\n", u, v, w, t);
	}
	for (int i = 1; i <= N; ++i)
		fprintf(fPointer, "%d %.2lf %.2lf\n", z[i], s[i], t[i]);
	fclose(fPointer);
}
void BaseData::ReadFromFile(const std::string &fname){
	try{
		FastReader frd(fname);
		V = frd.readint(), E = frd.readint(), M = frd.readint();
		L = frd.readint(), N = frd.readint(), Z = frd.readint();
		S = frd.read(), R = frd.read(), P = frd.read();
		for (int i = 1; i <= V; ++i)
			vertex[i].x = frd.read(), vertex[i].y = frd.read();
		for (int i = 1; i <= E; ++i){
			int u = frd.readint(), v = frd.readint();
			double w = frd.read(), t = frd.read();
			edge[std::make_pair(u, v)] = std::make_pair(w, t);
		}
		for (int i = 1; i <= N; ++i){
			z[i] = frd.readint();
			s[i] = frd.read(), t[i] = frd.read();
		}
	} catch (MyException &e){
		puts(e.what());
	}
}

