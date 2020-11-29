ID = 20190673

echo "EE209 SUBMIT >> Generating Submission Files for Assignment 3 ..."
echo "Removing Outdated Files..."
rm -f ./submission/*.c
rm -f ./submission/*.s
rm -f ./submission/readme
rm -f ./submission/*tar.gz

cp mydc.s ./submission/
cp test_* ./submission/
cp readme ./submission/

cd ./submission/
source ~/.bashrc

tar -zcf 20190673_assign4.tar.gz *

echo "EE209 SUBMIT >> Submission file successfully generated"