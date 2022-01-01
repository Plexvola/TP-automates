#include "afd.h"
#include "afn.h"
#include <stdio.h>
#define GREEN "\e[0;32m"
#define RED "\e[0;31m"
#define RESET "\e[0m"

int main(int argc, char *argv[])
{
	afn U;
	afd D;
	char var[] = {96, 0};
	int c = 1;
	for (int i = 0; i < 26 && c; i++) {
		var[0]++;
		printf("test avec %s\n", var);
		afn_char(&U, var[0]);
		afn_determinisation(U, &D);
		if (afd_simul(var, D) && var[0] == i + 97) {
			puts(GREEN "Valide" RESET);
		} else {
			c = 0;
			afn_print(U);
			puts(RED "Invalide" RESET);
		}
	}
	if (c) {
		puts(GREEN "Tous les tests sont corrects" RESET);
	} else {
		printf(RED "Erreur avec %s\n" RESET, var);
	}
	return 0;
}
