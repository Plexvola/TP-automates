#include "afd.h"
#include "afn.h"
#include <stdio.h>
#define GREEN "\e[0;32m"
#define RED "\e[0;31m"
#define RESET "\e[0m"

int main(int argc, char *argv[])
{
	afn X;
	afn Y;
	afn Z;
	afd D;
	char var1[] = {96, 0};
	int c = 1;
	char var2[] = {96, 0};
	for (int i = 0; i < 26; i++) {
		var1[0] = i + 97;
		for (int j = 0; j < 26; j++) {
			var2[0] = j + 97;
			printf("test avec %s puis %s\n", var1, var2);
			afn_char(&X, var1[0]);
			afn_char(&Y, var2[0]);
			afn_union(&Z, X, Y);
			afn_determinisation(Z, &D);
			int c1 = afd_simul(var1, D);
			int c2 = afd_simul(var2, D);
			int c3 = (var1[0] == i + 97) && (var2[0] == j + 97);
			printf("%s:%d\n", var1, c1);
			printf("%s:%d\n", var2, c2);
			if (c1 && c2 && c3) {
				printf(GREEN "Valide\n" RESET);
			} else {
				c = 0;
				afn_print(Z);
				printf(RED "Invalide\n" RESET);
			}
		}
	}
	if (c) {
		printf(GREEN "Tous les tests sont corrects\n" RESET);
	} else {
		printf(RED "Erreur avec %s%s\n" RESET, var1, var2);
	}
	return 0;
}
