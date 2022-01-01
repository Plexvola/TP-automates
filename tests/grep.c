#include "afd.h"
#include "afn.h"
#include "compregex.h"
#include <stdio.h>
#include <stdlib.h>
#define GRN "\e[0;32m"
#define RED "\e[0;31m"
#define RESET "\e[0m"

int main(int argc, char *argv[])
{
	afn N;
	afd D;
	char *regex;
	char *s;
	int c = 1;
	int c1;
	int c_l[] = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
	             1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0};
	char test1[] = "a";
	char test2[] = "bb";
	char test3[] = "cab";
	char *l_test[] = {test1, test2, test3};
	char automa1[] = "a";
	char automa2[] = "a+a";
	char automa3[] = "(a+a)";
	char automa4[] = "(a+b)";
	char automa5[] = "(c+a)";
	char automa6[] = "(c+a).(c+a)";
	char automa7[] = "(c+a).(c+a).b*";
	char automa8[] = "b{2}+[abc]";
	char *l_auto[] = {automa1, automa2, automa3, automa4,
	                  automa5, automa6, automa7, automa8};
	int cpt = 0;
	for (int i = 0; i < 8 && c; i++) {
		for (int j = 0; j < 3 && c; j++) {
			regex = l_auto[i];
			s = l_test[j];
			int n = strlen(regex);
			lex_unit *l = scanner(regex, n);
			N = parser(l, n);
			afn_determinisation(N, &D);
			c1 = afd_simul(s, D);
			printf("Test avec le regex:%s et la chaine[%s]\n", regex, s);
			if (c1 == c_l[cpt]) {
				printf(GRN "Valide\n" RESET);
			} else {
				c = 0;
				afn_print(N);
				printf(RED "Invalide\n" RESET);
			}
			cpt++;
		}
	}
	if (c) {
		printf(GRN "Tous les tests sont corrects\n" RESET);
	} else {
		printf(RED "Erreur avec le regex:%s et la chaine [%s]" RESET, regex, s);
	}

	return 0;
}
