#include "afn.h"
#include "afd.h"
#include "pile.h"
#include "file.h"
#include <stdio.h>
#include <string.h>


void afn_init(afn * A, uint nbetat, char *alphabet, ullong init,
			  ullong finals)
{
	A->nbetat = nbetat;

	A->alphabet = malloc(strlen(alphabet) + 1);
	strcpy(A->alphabet, alphabet);
	A->nbsymb = strlen(alphabet);

	A->init = init;
	A->finals = finals;

	int i;
	uchar symb;

	for (i = 0; i < SYMB_NB_MAX; i++)
		A->tsymb[i] = SYMB_NONE;

	for (i = 0; i < A->nbsymb; i++) {
		if ((alphabet[i] < SYMB_ASCII_DEB) ||
			(alphabet[i] > SYMB_ASCII_FIN)) {
			fprintf(stderr,
					"[afd_init] caractere ascii numero %d invalide\n",
					alphabet[i]);
			exit(-1);
		}
		symb = (uchar) (alphabet[i] - SYMB_ASCII_DEB);
		if (A->tsymb[symb] != SYMB_NONE) {
			fprintf(stderr,
					"[afd_init] caractere <%c> deja defini (ignorer)\n",
					alphabet[i]);
		} else {
			A->tsymb[symb] = i;
		}
	}
	A->delta = calloc(nbetat, sizeof(ullong *));
	for (i = 0; i < nbetat; i++) {
		A->delta[i] = calloc(A->nbsymb, sizeof(ullong));
	}
}

  /*
     Ajoute la relation (<q1>, <s>, <q2>) a l'AFN <A> 
   */
void afn_add_trans(afn * A, uint q1, uint s, uint q2)
{
	uchar symb = A->tsymb[s - SYMB_ASCII_DEB];

	if (symb == SYMB_NONE) {
		fprintf(stderr, "[add_trans] %u -- %c --> %u symbole inconnu\n",
				q1, s, q2);
		exit(-1);
	}

	if ((q1 < 0) || (q1 >= A->nbetat)) {
		fprintf(stderr, "[add_trans] etat <%d> non reconnu\n", q1);
		exit(-1);
	}
	if ((q2 < 0) || (q2 >= A->nbetat)) {
		fprintf(stderr, "[add_trans] etat <%d> non reconnu\n", q2);
		exit(-1);
	}

	A->delta[q1][symb] |= INT_ETAT(q2);
}


/*
  Libere la memoire de l'AFN <A>
*/
void afn_free(afn * A)
{
	free(A->alphabet);

	int i;
	for (i = 0; i < A->nbetat; i++)
		free(A->delta[i]);
	free(A->delta);
}


/*
  Affiche l'AFN <A>
*/
void afn_print(afn A)
{
	uint q, q1, s;
	printf("init:");
	for (q = 0; q < A.nbetat; q++) {
		if IN
			(q, A.init)
				printf(" %d", q);
	}
	printf("\n");
	printf("finals:");
	for (q = 0; q < A.nbetat; q++) {
		if IN
			(q, A.finals)
				printf(" %d", q);
	}
	printf("\n");
	for (q = 0; q < A.nbetat; q++) {
		for (s = 0; s < A.nbsymb; s++) {
			if (A.delta[q][s] != 0) {
				printf("%d -- %c --> {", q, A.alphabet[s]);
				for (q1 = 0; q1 < A.nbetat; q1++) {
					if (IN(q1, A.delta[q][s]))
						printf("%d,", q1);
				}
				printf("\b}\n");
			}
		}
	}
}



/*
  Initialise l'AFN <A> a partir des donnees du fichier <nomfichier> au
  format:

  nb_etats alphabet
  etat_initial
  nb_etats_finals
  etat_final_1 etat_final_2 ... etat_final_n
  etat_i_11 symb_j_1 etat_i_12
  etat_i_21 symb_j_2 etat_i_22
  .
  .
  .
  etat_i_n1 symb_j_n etat_i_n2

*/
void afn_finit(char *nomfichier, afn * A)
{
	FILE *f;
	f = fopen(nomfichier, "r");
	uint nbetat;
	char alphabet[64];
	ullong init;
	uint nbfinal;
	uint final;
	ullong finals = 0;
	fscanf(f, "%u %s\n", &nbetat, alphabet);
	fscanf(f, "%llu\n", &init);
	fscanf(f, "%u\n", &nbfinal);
	for (int i = 0; i < nbfinal; ++i) {
		fscanf(f, "%u", &final);
		finals |= (1 << final);
	}
	afn_init(A, nbetat, alphabet, init, finals);

	uint etat1;
	char symb;
	uint etat2;
	while (fscanf(f, "%u %c %u", &etat1, &symb, &etat2) != EOF)
		afn_add_trans(A, etat1, symb, etat2);
}

/*
  Retourne l'epsilon fermeture de l'ensemble d'états <R> par
  l'automate <A>
*/
ullong afn_epsilon_fermeture(afn A, ullong R)
{
	int i;
	ullong ferm;
	uint q;
	pile p = NULL;
	for (i = 0; R >> i > 0; i++)
		if ((R >> i) % 2 != 0)
			empile(i, &p);
	ferm = R;
	while (p != NULL) {
		q = depile(&p);
		for (uint i = 0; i < A.nbetat; ++i) {
			if (IN(i, A.delta[q][A.tsymb['&' - SYMB_ASCII_DEB]])
				&& !IN(i, ferm)) {
				ferm |= INT_ETAT(i);
				empile(i, &p);
			}
		}
	}
	return ferm;
}


/*
  Calcule un automate deterministe equivalent à <A> et affecte le
  resultat a <D>. Les etats de l'afd sont renumerotés à partir de 1
*/
void afn_determinisation(afn A, afd * D)
{
	int i, j;
	uint *states;
	states = calloc(INT_ETAT(A.nbetat) - 1, sizeof(uint)); 	// dictionnaire états d'afn -> nom afd
	file taint = NULL;
	ullong p, q, accessed;

	q = afn_epsilon_fermeture(A, A.init);
	afd_init(D, 2, A.alphabet, 0, 1, NULL);
	states[q] = 1;
	enfile(q, &taint);
	while(taint != NULL) { // états non-marqués
		p = defile(&taint);
		for (i = 0; i < A.nbsymb; ++i) { // symboles
			if(A.alphabet[i] == '&'){
				continue;
			}
			accessed = 0ULL;
			for (j = 0; p >> j > 0; j++) { // éléments de l'ullong
				if(IN(j, p))
					accessed |= A.delta[j][A.tsymb[A.alphabet[i]-SYMB_ASCII_DEB]];
				if(IN(j, A.finals))
					afd_add_final(D, states[p]);
			}
			q = afn_epsilon_fermeture(A, accessed);
			if(states[q] == 0) {
				states[q] = D->nbetat;
				afd_add_state(D);
				enfile(q, &taint);
			}
			afd_add_trans(D, states[p], A.alphabet[i], states[q]);
		}
	}
}

/*
  Calcule l'automate qui reconnait le caractere <c> dans un alphabet a
  <nbsymb> symboles
*/
void afn_char(afn * C, char c)
{
	afn_init(C, 2, ALPHABET, INT_ETAT(0), INT_ETAT(1));
	afn_add_trans(C, 0, c, 1);
}

/*
  Calcule un automate qui reconnait l'union de <A> et <B>
*/
void afn_union(afn * C, afn A, afn B)
{
	afn_init(C, A.nbetat + B.nbetat + 1, ALPHABET, INT_ETAT(0), A.finals<<1 | B.finals<<(A.nbetat+1) );
	for(int i = 0; i < 64; i++)
		if(A.init >> i & 0x1)
			afn_add_trans(C, 0, '&', i+1);
	
	for(int i = 0; i < 64; i++)
		if(B.init >> i & 0x1)
			afn_add_trans(C, 0, '&', i+A.nbetat+1);

	for (int i = 0; i < A.nbetat; ++i) {
		for (int j = 0; j < A.nbsymb; ++j) {
			for(int k = 0; k < 64; ++k){
				if(A.delta[i][j] >> k & 0x1)
					afn_add_trans(C, i+1, A.alphabet[j], k+1);
			}
		}
	}
	for (int i = 0; i < B.nbetat; ++i) {
		for (int j = 0; j < B.nbsymb; ++j) {
			for(int k = 0; k < 64; ++k){
				if(B.delta[i][j] >> k & 0x1)
					afn_add_trans(C, A.nbetat+i+1, B.alphabet[j], A.nbetat+k+1);
			}
		}
	}
}

/*
  Calcule un automate qui reconnait la concatenation de <A> et <B>
*/
void afn_concat(afn * C, afn A, afn B)
{
	afn_init(C, A.nbetat + B.nbetat, ALPHABET, A.init, B.finals);
	for (int a = 0; a < 64; a++) {
		if(A.finals >> a & 0x1)
			for (int b = 0; b < 64; b++) {
				if(B.init >> b & 0x1)
					afn_add_trans(C, a, '&', A.nbetat+b);
			}
	}
	for (int i = 0; i < A.nbetat; ++i) {
		for (int j = 0; j < A.nbsymb; ++j) {
			for(int k = 0; k < 64; ++k){
				if(A.delta[i][j] >> k & 0x1)
					afn_add_trans(C, i, A.alphabet[j], k);
			}
		}
	}
	for (int i = 0; i < B.nbetat; ++i) {
		for (int j = 0; j < B.nbsymb; ++j) {
			for(int k = 0; k < 64; ++k){
				if(B.delta[i][j] >> k & 0x1)
					afn_add_trans(C, A.nbetat+i, B.alphabet[j], A.nbetat+k);
			}
		}
	}
}

/*
  Calcule un automate qui reconnait la fermeture de Kleene de <A>
*/
void afn_kleene(afn * C, afn A)
{
	afn_init(C, A.nbetat + 1, ALPHABET, INT_ETAT(0), INT_ETAT(0));

	for(int i = 0; i < 64; i++)
		if(A.init >> i & 0x1)
			afn_add_trans(C, 0, '&', i+1);

	for(int i = 0; i < 64; i++)
		if(A.finals >> i & 0x1)
			afn_add_trans(C, i+1, '&', 0);

	for (int i = 0; i < A.nbetat; ++i) {
		for (int j = 0; j < A.nbsymb; ++j) {
			for(int k = 0; k < 64; ++k){
				if(A.delta[i][j] >> k & 0x1)
					afn_add_trans(C, i+1, A.alphabet[j], k+1);
			}
		}
	}
}
