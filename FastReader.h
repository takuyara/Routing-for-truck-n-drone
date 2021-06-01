//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#pragma once
#include "config.h"
#include <cstdio>
#include <string>
class FastReader{
	char buffer[maxreadbuf], *s, *t;
	FILE *fPointer;
	char getchar();
public:
	FastReader(const std::string &);
	virtual ~FastReader();
	double read();
	int readint();
} ;

