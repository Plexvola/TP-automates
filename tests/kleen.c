#include "afd.h"
#include "afn.h"
#include <stdio.h>
#define GREEN "\e[0;32m"
#define RED "\e[0;31m"
#define RESET "\e[0m"

int main(int argc, char *argv[])
{
	afn X;
	afn Z;
	afd D;
	int c = 1;
	char var1[] = {97, 0};
	char alpha[26] = "";
	for (int i = 0; i < 26 && c; i++) {
		printf("test avec %s ", var1);
		printf("et la chaine %s\n", alpha);
		afn_char(&X, var1[0]);
		afn_kleene(&Z, X);
		afn_determinisation(Z, &D);
		int c1 = afd_simul(alpha, D);
		if (c1) {
			puts(GREEN "Valide" RESET);
		} else {
			puts(RED "Invalide" RESET);
			afn_print(Z);
			printf(RED "Erreur avec %s et la chaine %s" RESET, var1, alpha);
			return 1;
		}
		alpha[i] = 97;
	}
	char alpha2[] = "bbbbbbbbbba";
	printf("test avec %s ", var1);
	printf("et la chaine %s\n", alpha2);
	afn_char(&X, var1[0]);
	afn_kleene(&Z, X);
	afn_determinisation(Z, &D);
	int c1 = afd_simul(alpha2, D);
	if (!c1) {
		puts(GREEN "Faux donc... Valide" RESET);
	} else {
		puts(RED "Invalide" RESET);
		afn_print(Z);
		printf(RED "Erreur avec %s et la chaine %s" RESET, var1, alpha);
		return 1;
	}
	puts(GREEN "Tous les tests sont corrects" RESET);
	return 0;
}
