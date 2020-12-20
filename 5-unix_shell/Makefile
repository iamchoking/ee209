TARGET=ish
gcc209 = gcc -Wall -Werror -ansi -pedantic -std=c99
COMPILER=$(gcc209)
FLAGS=-g -D_DEFAULT_SOURCE -D_GNU_SOURCE

#pattern
%.o: %.c
	$(COMPILER) $(FLAGS) -c $<

#TOP-LEVEL
all: $(TARGET)
ish: ish.o lex.o dynarray.o
	$(COMPILER) $(FLAGS) $? -o $@

lex_main: dynarray.o lex_main.o lex.o
	$(COMPILER) $(FLAGS) $? -o $@

syn_main: dynarray.o syn_main.o lex.o syn.o
	$(COMPILER) $(FLAGS) $? -o $@

#.o LEVEL
ish.o: lex.h dynarray.h

# dynarray.o : dynarray.h
# lex.o : lex.h dynarray.h
# lex.o: lex.c dynarray.c lex.h dynarray.h
# lex_main.o : lex.c dynarray.c lex.h dynarray.h
#lex_main.o : lex.c dynarray.c lex.h dynarray.h

#clean level
clean:
	rm -f *.o

scrub:
	rm -f *.o