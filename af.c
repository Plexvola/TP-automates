/*
  I53 Compilation et théorie des langages
  Nicolas Méloni
  28 - 10 -2019
 */
#include <stdio.h>
#include "afd.h"
#include "afn.h"

int main(int argc, char *argv[])
{
  afd A, B;
  char alphabet[] = "ab";
  uint finals[2] = {1,4};
  
  afd_init(&A, 5, alphabet, 2, 1, finals);
  
  afd_add_trans(&A, 1, 'a', 2);
  afd_add_trans(&A, 1, 'b', 1);
  afd_add_trans(&A, 2, 'a', 3);
  afd_add_trans(&A, 2, 'b', 1);
  afd_add_trans(&A, 3, 'a', 4);
  afd_add_trans(&A, 3, 'b', 1);
  afd_add_trans(&A, 4, 'a', 4);
  afd_add_trans(&A, 4, 'b', 4);

  afd_print(A);
  afd_finit(&B, "exemple.afd");
  afd_print(B);

  int t = afd_simul("b",B);
  if(t == 1)
	  printf("bravo\n");
  else
	  printf("non\n");
  afd_free(&B);
  afd_free(&A);

  afn X;
  afn_finit("exemple.afn", &X);
  afn_print(X);

  ullong k = afn_epsilon_fermeture(X, 2ULL);
  printf("%llu\n", k);

  return 0;
}


