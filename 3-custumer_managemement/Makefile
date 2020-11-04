SHELL = /bin/sh

gcc209 = gcc -Wall -Werror -ansi -pedantic -std=c99

all: testclient.c customer_manager1.c customer_manager2.c
	${gcc209} testclient.c customer_manager1.c -o testclient1
	${gcc209} testclient.c customer_manager2.c -o testclient2


1: testclient.c customer_manager1.c
	${gcc209} testclient.c customer_manager1.c -o testclient1


2: testclient.c customer_manager2.c
	${gcc209} testclient.c customer_manager2.c -o testclient2

1debug: testclient.c customer_manager1.c
	${gcc209} -g testclient.c customer_manager1.c -o 1d_testclient1


2degug: testclient.c customer_manager2.c
	${gcc209} -g testclient.c customer_manager2.c -o 2d_testclient2


clean:
	rm -f *.o a.out

scrub:
	rm -f *.o testclient1 testclient2 1d_testclient1 2d_testclient2