#include "SuffixArray2.h"
#include <bits/stdc++.h>
using namespace std;

struct Cmp{
	const char* text;
	Cmp(const char* t): text(t){}

	//retorna se o sufixo text[a..] < text[b..]
	bool operator()(int a, int b){
		int h = 0;
		while(text[a+h] && text[b+h] && text[a+h] == text[b+h]) ++h;
		if(!text[a+h]) return text[b+h];
		if(!text[b+h]) return false;
		return text[a+h] < text[b+h];
	}
};

int* buildSuffixArrayBruteForce(const char* text, size_t n) {
	int* sa = new int[n];
	for(int i = 0; i < n; ++i)
		sa[i] = i;
	
	sort(sa, sa + n, Cmp(text));

	return sa;
}

int getLcp(const char* ta, const char* tb){
	int lcp = 0;
	while(*ta && *tb && *ta == *tb) ++lcp, ++ta, ++tb;
	return lcp;
}

void fillLRcps(int l, int r, const char* text, int* sa,  int* lLcp, int* rLcp){
	if(r-l<=1) return;
	int h = (l+r)>>1;
	lLcp[h] = getLcp(text + sa[l], text + sa[h]);
	rLcp[h] = getLcp(text + sa[h], text + sa[r]);
	fillLRcps(l, h, text, sa, lLcp, rLcp);
	fillLRcps(h, r, text, sa, lLcp, rLcp);
}

void buildLRcpsBruteForce(const char* text, size_t n, int* sa, int* lLcp, int* rLcp){
	fillLRcps(0, n-1, text, sa, lLcp, rLcp);	
}

bool compareArrays(stringstream& result, int* expected, int* actual, size_t n){
	for(int i = 0; i < n; ++i){
		if(expected[i] != actual[i]){
			result << "Arrays diferem!\n";
			result << "\tEsperado:";
			for(int j = 0; j < n; ++j) result << " " << expected[j];
			result << "\n";
			result << "\tObtido:";
			for(int j = 0; j < n; ++j) result << " " << actual[j];
			result << "\n";
			return false;
		}
	}

	return true;
}

string test(const char* text, size_t n){
	stringstream result;
		
	int *saB = buildSuffixArrayBruteForce(text, n);
	SuffixArray2 sa;
	sa.build(text, n);
	
	result << "Testando o Array de Sufixos:";
	if(compareArrays(result, saB, sa.getArray(), n)){
		result << "Ok!\n";

		int* lLcp = new int[n], *rLcp = new int[n];
		for(int i = 0; i < n; ++i)
			lLcp[i] = rLcp[i] = -1;
				
		buildLRcpsBruteForce(text, n, saB, lLcp, rLcp);
		
		result << "Testando Llcp:";
		if(compareArrays(result, lLcp, sa.getLlcp(), n)){
			result << "Ok!\n";

			result << "Testando Rlcp:";
			if(compareArrays(result, rLcp, sa.getRlcp(), n)) result << "Ok!\n";
		}

		delete [] lLcp;
		delete [] rLcp;
	}

	delete saB;
	
	return result.str();
}

int main(){
	string ts[] = {"mississippi", "banana", "baoba", "abc", "ovo", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};

	for(int i = 0; i < 6; ++i){
		cout << "Testando texto |" << ts[i] << "|\n";
		cout << test(ts[i].c_str(), ts[i].size()) << endl;
	}
}


