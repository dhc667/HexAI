FILE_DIR=$(cd "$(dirname "$0")" && pwd)

# g++ dijkstra_board_test.cpp -o dijkstra_board_test
g++ -std=c++11  -o dijkstra_board_test $FILE_DIR/dijkstra_board_test.cpp
./dijkstra_board_test

rm dijkstra_board_test
