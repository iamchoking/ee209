gcc209 := 'gcc -Wall -Werror -ansi -pedantic -std=c99'

all: dc.o
	gcc -Wall -Werror -ansi -pedantic -std=c99 -m32 dc.o -o dc

dc_debug: dc.o
	gcc -Wall -Werror -ansi -pedantic -std=c99 -g -m32 dc.o -o dc
	rm -rf dc.o

dc.o: mydc.s
	as --32 -g mydc.s -o dc.o

clean:
	rm -rf dc dc.o
