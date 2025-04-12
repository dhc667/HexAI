#include "models/cell.hpp"
#include "monte_carlo/monte_carlo_tree_search.hpp"
// #include "monte_carlo_dsu/monte_carlo_tree_search.hpp"
#include "utils/dijkstra_board.hpp"

#include <iostream>
#include <vector>

extern "C" {
Cell get_move(short **board, unsigned int board_size, short player_id,
              unsigned int milliseconds) {
  return MCTS_get_move(board, board_size, player_id, milliseconds, 2);
}

bool game_over(short **_board, unsigned int board_size) {
  Board board(_board, board_size);
  return _game_over(board);
}

bool is_winner(short **_board, unsigned int board_size, short player_id) {
  Board board(_board, board_size);
  return player_won(board, player_id);
}
}

void basic_test() {
  std::vector<std::vector<short>> board_data{{1, 0, 0, 1, 2},
                                               {1, 1, 1, 2, 1},
                                                 {0, 0, 2, 1, 0},
                                                   {0, 2, 1, 0, 0},
                                                     {2, 0, 0, 0, 0}};

  Board board(board_data);

  // auto move = get_move(short **board, unsigned int board_size, short
  // player_id, unsigned int milliseconds)
  short **board_ptr = new short *[board_data.size()];
  for (size_t i = 0; i < board_data.size(); ++i) {
    board_ptr[i] = new short[board_data[i].size()];
    for (size_t j = 0; j < board_data[i].size(); ++j) {
      board_ptr[i][j] = board_data[i][j];
    }
  }

  Cell move = get_move(board_ptr, board.row_size(), 1, 5000);
  std::cout << "Move: (" << move.row << ", " << move.col << ")" << std::endl;

  for (size_t i = 0; i < board_data.size(); ++i) {
    delete[] board_ptr[i];
  }
  delete[] board_ptr;
}

void basic_test_2() {
  std::vector<std::vector<short>> board_data{{0, 0, 0, 0, 0},
                                               {0, 0, 0, 0, 0},
                                                 {0, 0, 0, 0, 0},
                                                   {0, 0, 0, 1, 0},
                                                     {0, 0, 0, 0, 0}};

  Board board(board_data);

  // auto move = get_move(short **board, unsigned int board_size, short
  // player_id, unsigned int milliseconds)
  short **board_ptr = new short *[board_data.size()];
  for (size_t i = 0; i < board_data.size(); ++i) {
    board_ptr[i] = new short[board_data[i].size()];
    for (size_t j = 0; j < board_data[i].size(); ++j) {
      board_ptr[i][j] = board_data[i][j];
    }
  }

  Cell move = get_move(board_ptr, board.row_size(), 1, 5000);
  std::cout << "Move: (" << move.row << ", " << move.col << ")" << std::endl;

  for (size_t i = 0; i < board_data.size(); ++i) {
    delete[] board_ptr[i];
  }
  delete[] board_ptr;
}
void test20x20() {
  std::vector<std::vector<short>> board_data;
  for (int i = 0; i < 20; i++) {
    board_data.push_back(std::vector<short>());
    board_data[i].assign(20, 0);
  }

  short **board_ptr = new short *[board_data.size()];
  for (size_t i = 0; i < board_data.size(); ++i) {
    board_ptr[i] = new short[board_data[i].size()];
    for (size_t j = 0; j < board_data[i].size(); ++j) {
      board_ptr[i][j] = board_data[i][j];
    }
  }
  std::cerr << "Board built" << std::endl;

  Cell move = get_move(board_ptr, board_data.size(), 1, 100000);
  std::cout << "Move: (" << move.row << ", " << move.col << ")" << std::endl;

  for (size_t i = 0; i < board_data.size(); ++i) {
    delete[] board_ptr[i];
  }
  delete[] board_ptr;
}

void test_empty_4x4() {
  std::vector<std::vector<short>> board_data{{0, 0, 0, 0},
                                               {0, 0, 0, 0},
                                                 {0, 0, 0, 0},
                                                   {0, 0, 0, 0}};

  Board board(board_data);

  // auto move = get_move(short **board, unsigned int board_size, short
  // player_id, unsigned int milliseconds)
  short **board_ptr = new short *[board_data.size()];
  for (size_t i = 0; i < board_data.size(); ++i) {
    board_ptr[i] = new short[board_data[i].size()];
    for (size_t j = 0; j < board_data[i].size(); ++j) {
      board_ptr[i][j] = board_data[i][j];
    }
  }

  Cell move = get_move(board_ptr, board.row_size(), 1, 50000);
  std::cout << "Move: (" << move.row << ", " << move.col << ")" << std::endl;

  for (size_t i = 0; i < board_data.size(); ++i) {
    delete[] board_ptr[i];
  }
  delete[] board_ptr;
}

int main() {
  // test20x20();
  // basic_test_2();
  test_empty_4x4();


  return 0;
}
