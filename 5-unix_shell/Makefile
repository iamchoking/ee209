TARGET=ish
gcc209 = gcc -Wall -Werror -ansi -pedantic -std=c99
COMPILER=$(gcc209)
FLAGS=-g -D_DEFAULT_SOURCE -D_GNU_SOURCE


#TOP-LEVEL
all: $(TARGET)
ish: dynarray.o ish.o syn.o lex.o exec.o sig.o
	$(COMPILER) $(FLAGS) $^ -o $@

lex_main: dynarray.o lex_main.o lex.o
	$(COMPILER) $(FLAGS) $^ -o $@

syn_main: dynarray.o syn_main.o lex.o syn.o
	$(COMPILER) $(FLAGS) $^ -o $@

exec_main: dynarray.o exec_main.o syn.o lex.o exec.o
	$(COMPILER) $(FLAGS) $^ -o $@

#.o level: pattern
%.o: %.c %.h
	$(COMPILER) $(FLAGS) -c $<

#clean level
clean:
	rm -f *.o out_*

scrub:
	rm -f *.o out_* lex_main syn_main exec_main ish