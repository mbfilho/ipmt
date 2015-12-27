#ifndef PRINTER_H
#define PRINTER_H

#include "Global.h"

class Printer {
public:
	Printer(const char* text, size_t n, const char* pat, size_t m);

	void addMatching(int matchingStart);
	void print();

private:
	/*
	 * Uma linha é um par (inicio, fim) de índices do texto.
	 * Este mapa guarda, para cada linha, o conjunto das posições iniciais das ocorrências do padrão na linha.
	*/	
	map<pair<int,int>, set<int> > linesOccs;
	
	const char* text;
	size_t n;

	const char* pat;
	size_t m;

	void getLine(int matchingStart); 
};

#endif
