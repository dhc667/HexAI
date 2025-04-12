#pragma once

#include "cell.hpp"
#include <string>
#include <vector>

#if DEBUG
#include <iostream>
#include <stdexcept>
#endif

class Board {
private:
  std::vector<std::vector<short>> board;

  short &operator[](const Cell &c) {
#if DEBUG
    if (!is_valid_cell(c)) {
      throw std::out_of_range("Invalid cell");
    }
#endif

    return this->board[c.row][c.col];
  }

public:
  Board(std::vector<std::vector<short>> &_board) { this->board = _board; }
  Board(short **_board, unsigned int board_size) {
    board.resize(board_size);
    for (unsigned int i = 0; i < board_size; i++) {
      board[i].resize(board_size);
      for (unsigned int j = 0; j < board_size; j++) {
        board[i][j] = _board[i][j];
      }
    }


  }

  Board copy() {
    std::vector<std::vector<short>> copy;
    copy.reserve(this->row_size());
    for (int i = 0; i < this->row_size(); i++) {
      copy[i].reserve(this->row_size());
      for (int j = 0; j < this->row_size(); j++) {
        copy[i][j] = (*this)[{i, j}];
      }
    }

    return Board(copy);
  }

  size_t row_size() const { return this->board.size(); }

  size_t cell_amount() const { return this->board.size() * this->board.size(); }

  std::vector<int> get_adjacents_from_id(int node_id, short player_id) const {
    static int dx[6]{0, 0, 1, 1, -1, -1};
    static int dy[6]{-1, 1, -1, 0, 1, 0};

#if DEBUG
    if (!is_valid_id(node_id)) {
      throw std::out_of_range("Invalid node id");
    }
#endif

    std::vector<int> adjacents;
    Cell c = from_node_id(node_id);

    for (int i = 0; i < 6; i++) {
      auto row = c.row + dx[i];
      auto col = c.col + dy[i];

      if (is_valid_cell({row, col}) &&
          (is_free_cell({row, col}) ||
           is_self_occupied_cell({row, col}, player_id))) {
        adjacents.push_back(to_node_id({row, col}));
      }
    }

    return adjacents;
  }

  std::vector<int> get_source_sink_adjacents(short player_id,
                                             bool source) const {
    std::vector<int> adjacents;

    bool horizontal_player = player_id == 1;
    for (int i = 0; i < this->board.size(); i++) {
      Cell c;
      if (horizontal_player) {
        c.row = i;
        if (source) {
          c.col = 0;
        } else {
          c.col = this->board.size() - 1;
        }
      } else {
        c.col = i;
        if (source) {
          c.row = 0;
        } else {
          c.row = this->board.size() - 1;
        }
      }

      if (is_free_cell(c) || is_self_occupied_cell(c, player_id)) {
        adjacents.push_back(to_node_id(c));
      }
    }

    return adjacents;
  }

  std::vector<int> get_valid_moves() const {
    std::vector<int> valid_moves;

    for (int i = 0; i < this->board.size(); i++) {
      for (int j = 0; j < this->board.size(); j++) {
        Cell c = {i, j};
        if (is_free_cell(c)) {
          valid_moves.push_back(to_node_id(c));
        }
      }
    }

    return valid_moves;
  }

  bool is_free_id(int node_id) const {
    return is_free_cell(from_node_id(node_id));
  }

  bool is_free_cell(Cell c) const {
#if DEBUG
    if (!is_valid_cell(c)) {
      throw std::out_of_range("Invalid cell");
    }
#endif

    return (*this)[c] == 0;
  }

  bool is_self_occupied_id(int node_id, short player_id) const {
    return is_self_occupied_cell(from_node_id(node_id), player_id);
  }

  bool is_self_occupied_cell(Cell c, short player_id) const {
#if DEBUG
    if (!is_valid_cell(c)) {
      throw std::out_of_range("Invalid cell 1: " + cell_to_str(c));
    }
#endif

    return (*this)[c] == player_id;
  }

  Cell from_node_id(int node_id) const {
#if DEBUG
    if (!is_valid_id(node_id)) {
      throw std::out_of_range("Invalid node id");
    }
#endif

    Cell answ;
    answ.row = node_id / this->board.size();
    answ.col = node_id % this->board.size();

    // #if DEBUG
    //     std::cerr << "from_node_id: " << node_id << " -> " << answ.row << ",
    //     "
    //               << answ.col << std::endl;
    // #endif

    return answ;
  }

  std::string to_string() const {
    int indent = 0;
    std::string str = "";
    for (int i = 0; i < this->board.size(); i++) {
      str += std::string(indent, ' ');
      for (int j = 0; j < this->board.size(); j++) {
        str += std::to_string((*this)[{i, j}]) + " ";
      }
      indent++;
      str += "\n";
    }
    return str;
  }

  int to_node_id(Cell c) const {
#if DEBUG
    if (!is_valid_cell(c)) {
      throw std::out_of_range("Invalid cell2: " + cell_to_str(c));
    }
#endif

    return c.row * this->board.size() + c.col;
  }

  bool is_valid_cell(Cell c) const {
    return c.row >= 0 && c.row < this->board.size() && c.col >= 0 &&
           c.col < this->board.size();
  }

  bool is_valid_id(int id) const {
    return id >= 0 && id < this->board.size() * this->board.size();
  }

  void make_move(Cell c, short player_id) {
#if DEBUG
    if (!is_valid_cell(c)) {
      throw std::out_of_range("Invalid cell3: " + cell_to_str(c));
    }
    if (!is_free_cell(c)) {
      throw std::invalid_argument("Cell is not free");
    }
#endif

    (*this)[c] = player_id;
  }

  void undo_move(Cell c) {
#if DEBUG
    if (!is_valid_cell(c)) {
      throw std::out_of_range("Invalid cell4: " + cell_to_str(c));
    }
#endif

    (*this)[c] = 0;
  }


  short get_value_at(Cell c) const { return (*this)[c]; }

  short operator[](Cell c) const {
#if DEBUG
    if (!is_valid_cell(c)) {
      throw std::out_of_range("Invalid cell5: " + cell_to_str(c));
    }
#endif

    return this->board[c.row][c.col];
  }

  // short operator[](int id) const {
  // #if DEBUG
  //   if (!is_valid_id(id)) {
  //     throw std::out_of_range("Invalid node id");
  //   }
  // #endif

  //   Cell c = from_node_id(id);
  //   return (*this)[c];
  // }
};
