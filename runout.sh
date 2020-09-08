gcc -Wall -Werror -ansi -pedantic -std=c99 ${1}
./a.out < ${2:-mysample.txt} > output 2> errors