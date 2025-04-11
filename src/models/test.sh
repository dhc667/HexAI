FILE_DIR=$(cd "$(dirname "$0")" && pwd)
cd $FILE_DIR

g++ ./board_test.cpp -o board_test
./board_test

rm board_test
