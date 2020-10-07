# 1: file name (~.c) to compile (in the same directory) 2: assignment number 3: student id

#compiling process: ~.c >preprocess> ~.i >compile> ~.s >assemble> ~.o >link> ~
echo "Removing Old Compiles ... "
rm -f -v 1_client
rm -f -v 2_sgrep
rm -f -v a.out

gcc -Wall -Werror -ansi -pedantic -std=c99 client.c str.c -o 1_client
gcc -Wall -Werror -ansi -pedantic -std=c99 sgrep.c str.c -o 2_sgrep

echo "Successfully Compiled : 1_client, 2_sgrep"