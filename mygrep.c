#include "afd.h"
#include "afn.h"
#include "compregex.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc < 3){
		printf("usage: %s <exreg> <chaîne>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	afn N;
	afd D;
	char* regex = argv[1];
	char* s = argv[2];
	int n = strlen(regex);
	lex_unit* l = scanner(regex, n);
	N = parser(l, n);
	afn_determinisation(N, &D);
	if(afd_simul(s, D)){
		printf("accepté\n");
	} else {
		printf("rejeté\n");
	}
	return 0;
}
