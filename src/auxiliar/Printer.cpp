#include "Printer.h"

Printer::Printer(const char* text, size_t n, const char* pat, size_t m): text(text), n(n), pat(pat), m(m) {}

/*
* Acrescenta a ocorrência que se inicia em 'matchingStart' na linha correspondente
*/
void Printer::addMatching(int matchingStart) {
	int lineSt = matchingStart, lineEnd = matchingStart;

	while(lineSt > 0 && text[lineSt] != '\n') --lineSt;
	if(text[lineSt] == '\n') ++lineSt;
	
	while(lineEnd < n-1 && text[lineEnd] != '\n') ++lineEnd;
	if(text[lineEnd] == '\n') --lineEnd;

	linesOccs[make_pair(lineSt, lineEnd)].insert(matchingStart);
}

/*
* Imprime todas as ocorrencias registradas do padrão 'pat' no texto 'text'
*/
void Printer::print() {
	for(map<pair<int,int>, set<int> >::iterator it = linesOccs.begin(); it  != linesOccs.end(); ++it) {//Para toda linha
		int pos = (it->first).first, lastPos = (it->first).second; //os limites da linha atual

		for(set<int>::iterator occ = (it->second).begin(); occ != (it->second).end(); ++occ) {//Para toda ocorrencia na linha
			if(pos < *occ) { 
				printf("%.*s", *occ - pos, text + pos);
				printf("\033[31m");
				printf("%s", pat);
				printf("\033[0m");	
				pos = *occ + m;
			} else if(*occ + m > pos){
				printf("\033[31m");
				printf("%.*s", int(*occ + m - pos), pat + (pos-*occ));
				printf("\033[0m");	
				pos = *occ + m;
			}
		}

		if(pos <= lastPos)
			printf("%.*s", lastPos - pos + 1, text + pos);

		printf("\n");
	}
}


