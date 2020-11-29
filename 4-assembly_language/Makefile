gcc209 := 'gcc -Wall -Werror -ansi -pedantic -std=c99'

all: mydc.s
	as --32 -g mydc.s -o dc.o
	gcc -Wall -Werror -ansi -pedantic -std=c99 -g -m32 dc.o -o dc

dc: mydc.s
	as --32 -g mydc.s -o dc.o
	gcc -Wall -Werror -ansi -pedantic -std=c99 -g -m32 dc.o -o dc
	rm -rf dc.o


clean:
	rm -rf dc dc.o
