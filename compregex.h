#include "afn.h"

#define IS_ALPHABET(X) ((X >= '0' && X <= '9') || (X >= 'A' && X <= 'Z') || (X >= 'a' && X <= 'z'))

enum type { CHAR, BINOP, UNOP, PAR_OUV, PAR_FER };

typedef struct {
	enum type t;
	char c;
} lex_unit;

lex_unit* scanner(char *s, int len);
afn parser(lex_unit * s, int len);

afn regex();
afn concat();
afn kleene();
afn base();
