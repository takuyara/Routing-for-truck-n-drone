//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#include "FastReader.h"
#include "Exceptions.h"
#include "utils.h"
FastReader::FastReader(const std::string &fname){
	fPointer = fopen(fname.c_str(), "r");
	if (!fPointer) throw NullFileException();
	s = t = NULL;
}
FastReader::~FastReader(){
	fclose(fPointer);
}
char FastReader::getchar(){
	if (s == t) t = (s = buffer) + fread(buffer, 1, maxreadbuf, fPointer);
	if (s == t) throw EofException();
	return *s++;
}
double FastReader::read(){
	double x = 0, f = 1, bl = 1, flg = 1; char ch;
	while ((ch = getchar()) < '0' || ch > '9'){
		if (ch == '-') f = -1;
		if (ch == EOF) throw EofException();
	}
	while ('0' <= ch && ch <= '9'){
		x = x * 10 * flg + (ch - '0') * (bl *= flg);
		ch = getchar();
		if (ch == '.') flg = 0.1, ch = getchar();
	}
	return x * f;
}
int FastReader::readint(){
	return toint(read());
}

