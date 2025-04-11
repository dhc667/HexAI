#include "../models/board.hpp"
#include "dijkstra_board.hpp"
#include <cassert>
#include <iostream>

void print_distances(const std::vector<int> &d, const Board &board) {
  int indent = 0;
  for (size_t i = 0; i * i < d.size(); i++) {
    for (int k = 0; k < indent; k++) {
      std::cout << " ";
    }
    for (size_t j = 0; j * j < d.size(); j++) {
      auto distance =
          d[board.to_node_id({static_cast<int>(i), static_cast<int>(j)})];
      if (distance == INT_MAX) {
        std::cout << "X ";
      } else {
        std::cout << distance << " ";
      }
    }
    indent += 1;
    std::cout << std::endl;
  }

  std::cout << std::endl;
}

void basic_test() {
  std::vector<std::vector<short>> board_data{{0, 0, 0, 0, 0},
                                             {0, 1, 1, 1, 0},
                                             {0, 1, 0, 1, 0},
                                             {0, 0, 1, 1, 0},
                                             {0, 0, 0, 0, 0}};

  Board board(board_data);
  std::vector<int> d;
  source_sink_dijkstra_until_k(board, 1, 2, true, d);

  print_distances(d, board);

  assert(d[board.to_node_id({0, 0})] == 1);
  assert(d[board.to_node_id({0, 1})] == 3);
  assert(d[board.to_node_id({1, 1})] == 2);
}

void test_2() {
  // board
// 2 2 0 2 0
//  0 1 2 0 0
//   0 2 1 0 2
//    1 1 0 1 1
//     0 0 0 0 0
  
  std::vector<std::vector<short>> board_data{{2, 2, 0, 2, 0},
                                             {0, 1, 2, 0, 0},
                                             {0, 2, 1, 0, 2},
                                             {1, 1, 0, 1, 1},
                                             {0, 0, 0, 0, 0}};

  Board board(board_data);
  std::vector<int> d;
  source_sink_dijkstra_until_k(board, 1, 2, true, d);

  print_distances(d, board);
}

void game_over_test() {
  std::vector<std::vector<short>> board_data{{2, 2, 0, 2, 0},
                                               {0, 1, 2, 1, 1},
                                                 {0, 2, 1, 0, 2},
                                                   {1, 1, 0, 1, 1},
                                                     {0, 0, 0, 0, 0}};

  Board board(board_data);
  std::vector<int> d;
  source_sink_dijkstra_until_k(board, 1, 0, true, d);

  assert(_game_over(board) == true);

  print_distances(d, board);
}

void game_over_test_2() {
  std::vector<std::vector<short>> board_data{{2, 2, 0, 2, 0},
                                               {0, 1, 2, 1, 0},
                                                 {0, 2, 1, 0, 2},
                                                   {1, 1, 0, 1, 1},
                                                     {0, 0, 0, 0, 0}};

  Board board(board_data);
  std::vector<int> d;
  source_sink_dijkstra_until_k(board, 1, 0, true, d);

  assert(_game_over(board) == false);

  print_distances(d, board);
}
int main() {
  basic_test();
  test_2();
  game_over_test();
  game_over_test_2();

  std::cout << "All tests passed!" << std::endl;
  return 0;
}
