#include "compregex.h"
#include "afn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static lex_unit *reg;
static int pos;

// transforme un regex en une chaîne d'unités lexicales
lex_unit *scanner(char *s, int len)
{
	lex_unit *lchain = calloc(len, sizeof(lex_unit));
	lex_unit lex;
	for (int i = 0; i < len; ++i) {
		if (IS_ALPHABET(s[i]))
			lex.t = CHAR;
		else if (s[i] == '.' || s[i] == '+')
			lex.t = BINOP;
		else if (s[i] == '*')
			lex.t = UNOP;
		else if (s[i] == '(')
			lex.t = PAR_OUV;
		else if (s[i] == ')')
			lex.t = PAR_FER;
		else {
			fprintf(stderr, "%c: caractère inconnu\n", s[i]);
			exit(EXIT_FAILURE);
		}
		lex.c = s[i];
		lchain[i] = lex;
	}
	return lchain;
}

/* transforme une chaîne d'unités lexicales en AFN */
afn parser(lex_unit * s, int len)
{
	reg = s;
	pos = len-1;
	return regex();
}

afn regex()
{
	afn C, R;
	C = concat();
	if (reg[pos].t == BINOP && reg[pos].c == '+') {
		pos--;
		afn_union(&R, regex(), C);
	} else
		R = C;
	return R;
}

afn concat()
{
	afn K, R;
	K = kleene();
	if (reg[pos].t == BINOP && reg[pos].c == '.') {
		pos--;
		afn_concat(&R, concat(), K);
	} else
		R = K;
	return R;
}

afn kleene()
{
	afn A;
	if (reg[pos].t == UNOP && reg[pos].c == '*') {
		pos--;
		afn_kleene(&A, base());
	} else
		A = base();
	return A;
}

afn base()
{
	afn A;
	if (reg[pos].t == PAR_FER) {
		pos--;
		A = regex();
		if (reg[pos].t == PAR_OUV) {
			pos--;
			return A;
		}
		else {
			fprintf(stderr, "%d %c: parenthèse ouvrante attendue\n", pos,
					reg[pos].c);
			exit(EXIT_FAILURE);
		}
	} else if (reg[pos].t == CHAR)
		afn_char(&A, reg[pos].c);
	else {
		fprintf(stderr, "%c: non-légal (doit être un caractère ou une expression entre parenthèses)\n", reg[pos].c);
		exit(EXIT_FAILURE);
	}
	pos--;
	return A;
}
