#include "afd.h"
#include "afn.h"
#include "compregex.h"
#include <getopt.h>
#include <stdlib.h>
#define GREEN "\e[0;32m"
#define RED "\e[0;31m"
#define RESET "\e[0m"

int main(int argc, char *argv[])
{
	char opt;
	int verbose = 0;
	while ((opt = getopt(argc, argv, "hv")) != -1) {
		switch (opt) {
		case 'v':
			verbose = 1;
			break;

			printf("usage: %s [-v] <exreg> <chaîne>\n", argv[0]);
			exit(EXIT_SUCCESS);
		default:
			printf("usage: %s [-v] <exreg> <chaîne>\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	if (argc - optind < 2) {
		printf("usage: %s [-v] <exreg> <chaîne>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	afn N;
	afd D;
	char *regex_input = argv[optind];
	char *s = argv[optind + 1];
	int n = strnlen(regex_input, 256);
	lex_unit *l = scanner(regex_input, n);
	N = parser(l, n);
	free(l);
	if (verbose) {
		afn_print(N);
	}
	afn_determinisation(N, &D);
	afn_free(&N);
	int sim = afd_simul(s, D);
	afd_free(&D);
	if (sim) {
		puts(GREEN "accepté" RESET);
		return 0;
	} else {
		puts(RED "rejeté" RESET);
		return 1;
	}
}
