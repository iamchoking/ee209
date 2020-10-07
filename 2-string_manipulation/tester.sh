#1: testing compiled file #2: baseline compiled file #3: testing directory #4: checker compiled file

compareOutput(){
    echo -e "outputs :"
    diff output1 output2
    echo -e "errors  :"
    diff error1 error2 
}

#sgrep testing
echo -e "1. DoFind (format: ./~ -f [string] < (stdin redirection) )"
echo -e "1.1. Argument Validation"
    echo -e "standard case"
    ${1:-./sgrep_sol} -f    and       < ./test/microsoft.txt  > output1  2> error1
    ${2:-./2_sgrep}   -f    and       < ./test/microsoft.txt  > output2  2> error2
    compareOutput

    echo -e "line too long case"
    ${1:-./sgrep_sol} -f    and       < ./test/long1line.txt  > output1  2> error1
    ${2:-./2_sgrep}   -f    and       < ./test/long1line.txt  > output2  2> error2
    compareOutput

    echo -e "argument too long case"
    ${1:-./sgrep_sol} -f    "$(<./test/args/toolong)" < ./test/microsoft.txt  > output1  2> error1

    ${2:-./2_sgrep}   -f   "$(<./test/args/toolong)" < ./test/microsoft.txt  > output2  2> error2
    compareOutput

echo -e "\n1.2 String Search"

    echo -e "standard case"
    ${1:-./sgrep_sol} -f    and       < ./test/google.txt  > output1  2> error1
    ${2:-./2_sgrep}   -f    and       < ./test/google.txt  > output2  2> error2
    compareOutput

    echo -e "empty file"
    ${1:-./sgrep_sol} -f    and       < ./test/empty.txt  > output1  2> error1
    ${2:-./2_sgrep}   -f    and       < ./test/empty.txt  > output2  2> error2
    compareOutput

    echo -e "empty search string"
    ${1:-./sgrep_sol} -f    ""       < ./test/google.txt  > output1  2> error1
    ${2:-./2_sgrep}   -f    ""       < ./test/google.txt  > output2  2> error2
    compareOutput

    echo -e "null search string"
    ${1:-./sgrep_sol} -f    '\0'       < ./test/google.txt  > output1  2> error1
    ${2:-./2_sgrep}   -f    '\0'       < ./test/google.txt  > output2  2> error2
    compareOutput

echo -e "\n----------------------------------------------------------------\n"
echo -e "2. DoReplace()(format: ./~ -r [fromStr] [toStr] < (stdin redirection) )"

    echo -e "2.1. Argument Validatian"
    echo "standard case"
    ${1:-./sgrep_sol} -r    a b       < ./test/google.txt  > output1  2> error1
    ${2:-./2_sgrep}   -r    a b       < ./test/google.txt  > output2  2> error2
    compareOutput

    echo "stdin too long"
    ${1:-./sgrep_sol} -r    a b       < ./test/long1line.txt  > output1  2> error1
    ${2:-./2_sgrep}   -r    a b       < ./test/long1line.txt  > output2  2> error2
    compareOutput

    echo "fromStr too long"
    ${1:-./sgrep_sol} -r    "$(<./test/args/toolong)" b       < ./test/microsoft.txt  > output1  2> error1
    ${2:-./2_sgrep}   -r    "$(<./test/args/toolong)" b       < ./test/microsoft.txt  > output2  2> error2
    compareOutput

    echo "toStr too long"
    ${1:-./sgrep_sol} -r    a "$(<./test/args/toolong)"      < ./test/microsoft.txt  > output1  2> error1
    ${2:-./2_sgrep}   -r    a "$(<./test/args/toolong)"      < ./test/microsoft.txt  > output2  2> error2
    compareOutput

    echo "fromStr empty"
    ${1:-./sgrep_sol} -r    '' b      < ./test/microsoft.txt  > output1  2> error1
    ${2:-./2_sgrep}   -r    '' b      < ./test/microsoft.txt  > output2  2> error2
    compareOutput

    echo "stdin empty"
    ${1:-./sgrep_sol} -r    a b      < ./test/empty.txt  > output1  2> error1
    ${2:-./2_sgrep}   -r    a b      < ./test/empty.txt  > output2  2> error2
    compareOutput

    echo -e "\n2.2. String Replace"
    declare -a POOLFROM
    declare -a POOLTO
    POOLFROM=(a e i o u b '\0')
    POOLTO=(aa ee ei bo bbbbbb aaa a)
    for i in "${POOLFROM[@]}"
    do
        for j in "${POOLTO[@]}"
        do
            echo -e "\ntesting for $i to $j"
            ${1:-./sgrep_sol} -r    $i $j  < ./test/microsoft.txt  > output1  2> error1
            ${2:-./2_sgrep}   -r    $i $j  < ./test/microsoft.txt  > output2  2> error2
            compareOutput
        done
    done

echo -e "\n----------------------------------------------------------------\n"
echo -e "3. DoDiff()(format: ./~ -d [file1] [file2]"

# FILES=${3:-./test/*.txt}
#     for i in $FILES
#     do
#         for j in $FILES
#         do
#             echo -e "\ntesting for $i to $j"
#             ${1:-./sgrep_sol} -d    $i $j  > output1  2> error1
#             ${2:-./2_sgrep}   -d    $i $j  > output2  2> error2
#             compareOutput
#         done
#     done

echo -e "\nBasic"
${1:-./sgrep_sol} -d    ./test/empty.txt ./test/empty.txt  > output1  2> error1
${2:-./2_sgrep}   -d    ./test/empty.txt ./test/empty.txt  > output2  2> error2
compareOutput


# for f in $FILES
# do
#     echo -e "\n\ntesting for $f"
#     ${1:-./sgrep_sol}            < $f  > output1  2> error1
#     ${2:-./2_sgrep}              < $f  > output2  2> error2
#     echo "outputs :"
#     diff output1 output2
#     echo "errors  :"
#     diff error1 error2

# done

# rm -f output*
# rm -f error*