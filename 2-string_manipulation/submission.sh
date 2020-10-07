ID = 20190673

echo "EE209 SUBMIT >> Generating Submission Files for Assignment 2 ..."
echo "Removing Outdated Files..."
rm -f ./submission/*.c
rm -f ./submission/*.h
rm -f ./submission/readme
rm -f ./submission/*tar.gz

cp str.c ./submission/
cp str.h ./submission/
cp sgrep.c ./submission/
cp readme ./submission/

cd ./submission/
source ~/.bashrc

tar -zcf "$ID"_assign2.tar.gz *

echo "EE209 SUBMIT >> Submission file successfully generated"