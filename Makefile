CC=gcc
CFLAGS=-Wall -g -I./include
OBJ=pile.o file.o afd.o afn.o compregex.o
TESTS=determinisation_test.bin union_test.bin concat_test.bin kleen_test.bin grep_test.bin

all: mygrep test valgrind tags

test: mygrep $(TESTS)
	./mygrep "a" "a"
	not ./mygrep "a" "b"
	./mygrep "[abc]" "a"
	./mygrep "[abc]" "b"
	./mygrep "[abc]" "c"
	not ./mygrep "[abc]" "d"
	./mygrep "a{3}" "aaa"
	not ./mygrep "a{3}" "aaaa"
	./mygrep "r*" "rrrrrrrr"
	not ./mygrep "r*" "rrrurr"
	./mygrep "o.v" "ov"
	not ./mygrep "o.v" "ou"
	./mygrep "x+y" "x"
	./mygrep "x+y" "y"
	not ./mygrep "x+y" "z"
	for test in $(TESTS); do ./tests/$$test; done

valgrind: mygrep
	valgrind --suppressions=mygrep.supp --leak-check=full --show-leak-kinds=all ./mygrep "([at]*).(n{5}+r{2})" "attaatattnnnnn"

%_test.bin: ./tests/%.c $(OBJ)
	$(CC) $(CFLAGS) $^ -o tests/$@

mygrep: mygrep.o $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

af:af.o $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *~ *.bak tags af mygrep tests/*_test.bin

tags: af*.[hc] compregex.[hc] [pf]ile.[hc] mygrep.c
	ctags -R $^

lint:
	clang-format -i -style='{IndentWidth: 4, TabWidth: 4, UseTab: AlignWithSpaces, ColumnLimit: 80, AlignAfterOpenBracket: true, BreakBeforeBraces: Linux, PointerAlignment: Right}' *.c tests/*.c
