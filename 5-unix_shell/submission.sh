ID = 20190673

echo "EE209 SUBMIT >> Generating Submission Files for Assignment 5 ..."
echo "Removing Outdated Files..."
rm -f ./submission/*.c
rm -f ./submission/*.h
rm -f ./submission/readme
rm -f ./submission/Makefile
rm -f ./submission/*tar.gz
rm -f ./submission/*.o

cp alias.* ./submission/
cp dynarray.* ./submission/
cp exec.* ./submission/
cp ish_err.* ./submission/
cp ish.* ./submission/
cp lex.* ./submission/
cp redir.* ./submission/
cp sig.* ./submission/
cp syn.* ./submission/

cp readme ./submission/
cp Makefile ./submission/

rm -f ./submission/*.o

cd ./submission/
source ~/.bashrc

tar -zcf 20190673_assign5.tar.gz *

echo "EE209 SUBMIT >> Submission file successfully generated"