#1: testing compiled file #2: baseline compiled file #3: testing directory #4: checker compiled file
FILES=${3:-./sample/test_upload/*}

for f in $FILES
do
    echo -e "\n\ntesting for $f"
    ${1:-./wc209}            < $f  > output1  2> error1
    ${2:-sample/samplewc209} < $f  > output2  2> error2
    echo "outputs :"
    diff output1 output2
    echo "errors  :"
    diff error1 error2

done

bash ./clean.sh || true