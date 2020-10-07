# 1: file name (~.c) to compile (in the same directory) 2: assignment number 3: student id

#compiling process: ~.c >preprocess> ~.i >compile> ~.s >assemble> ~.o >link> ~
echo "Removing Compiles ... "

rm -f -v 1_client
rm -f -v 2_sgrep
rm -f -v a.out

echo "Removing Outputs/Error Streams..."
rm -f output*
rm -f error*
echo "Clean Complete"