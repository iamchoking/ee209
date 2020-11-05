ID = 20190673

echo "EE209 SUBMIT >> Generating Submission Files for Assignment 3 ..."
echo "Removing Outdated Files..."
rm -f ./submission/*.c
rm -f ./submission/*.h
rm -f ./submission/readme
rm -f ./submission/*tar.gz

cp customer_manager1.c ./submission/
cp customer_manager2.c ./submission/
cp customer_manager.h ./submission/
cp readme ./submission/

cd ./submission/
source ~/.bashrc

tar -zcf 20190673_assign3.tar.gz *

echo "EE209 SUBMIT >> Submission file successfully generated"