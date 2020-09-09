# 1: file name (~.c) to compile (in the same directory) 2: assignment number 3: student id
NAME=$(echo ${1:-noname.c}| cut -d'.' -f 1)
NUM=${2:-1}
ID=${3:-20190673}
echo "EE209 SUBMIT >> compiling sequentially for $NAME ..."
rm -f -r ./submission/*.c
rm -f -r ./submission/*.i
rm -f -r ./submission/*.s
rm -f -r ./submission/*.o
rm -f -r ./submission/$NAME
rm -f ./submission/readme
rm -f ./submission/*tar.gz

cp $NAME.c ./submission/
cp readme ./submission/

cd ./submission/
source ~/.bashrc

#compiling process: ~.c >preprocess> ~.i >compile> ~.s >assemble> ~.o >link> ~
gcc -Wall -Werror -ansi -pedantic -std=c99 -E $NAME.c > $NAME.i
gcc -Wall -Werror -ansi -pedantic -std=c99 -S $NAME.i
gcc -Wall -Werror -ansi -pedantic -std=c99 -c $NAME.s
gcc -Wall -Werror -ansi -pedantic -std=c99    $NAME.o -lc -o $NAME

tar -zcf "$ID"_assign"$NUM".tar.gz *

echo "EE209 SUBMIT >> Submission file successfully generated"