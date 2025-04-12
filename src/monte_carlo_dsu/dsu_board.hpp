#pragma once

#include "../models/board.hpp"
#include "dsu_undo.hpp"
#include <stack>
#include <string>

#ifdef DEBUG
#include <exception>
#include <iostream>
#endif // DEBUG

class DSUBoard {
private:
  Board board;

  const int SINK;
  const int SOURCE;

  std::stack<int> horizontal_changes_by_move;
  std::stack<int> vertical_changes_by_move;

  dsu_undo horizontal_board;
  dsu_undo vertical_board;

  std::stack<Cell> moves;

  int connect_to(int node_id, const std::vector<int> &adjacents,
                 short player_id) {
    int change_counter = 0;
    auto dsu =
        (player_id == 1) ? &this->horizontal_board : &this->vertical_board;

    for (auto adj : adjacents) {
      auto cell = board.from_node_id(adj);
      if (!board.is_self_occupied_cell(cell, player_id))
        continue;

      bool change_occurred = dsu->merge(node_id, adj);
      if (change_occurred)
        change_counter++;
    }

    return change_counter;
  }


  void initialize_dsu_for_player(short player_id) {
    auto dsu =
        (player_id == 1) ? &this->horizontal_board : &this->vertical_board;

    dsu->new_dsu(this->board.cell_amount() + 2);

    auto source_adjacents = board.get_source_sink_adjacents(player_id, true);
    connect_to(this->SOURCE, source_adjacents, player_id);

    auto sink_adjacents = board.get_source_sink_adjacents(player_id, false);
    connect_to(this->SINK, sink_adjacents, player_id);

    for (int i = 0; i < this->board.row_size(); i++) {
      for (int j = 0; j < this->board.row_size(); j++) {
        if (!board.is_self_occupied_cell({i, j}, player_id))
          continue;

        auto cell_id = board.to_node_id({i, j});
        auto adjacents = board.get_adjacents_from_id(cell_id, player_id);
        connect_to(cell_id, adjacents, player_id);
      }
    }

#ifdef DEBUG
    if (dsu->setOf(this->SOURCE) == dsu->setOf(this->SINK)) {
      std::cerr << "Board initially terminal" << std::endl;
      throw std::exception();
    }
#endif // DEBUG
  }

public:
  DSUBoard(Board &board)
      : SOURCE(board.cell_amount()), SINK(board.cell_amount() + 1),
        board(board) {
    initialize_dsu_for_player(1);
    initialize_dsu_for_player(2);
  }

  std::string to_string() {
    return this->board.to_string();
  }
  void make_move(Cell c, short player_id) {
#ifdef DEBUG
    if (!board.is_free_cell(c)) {
      std::cerr << "Trying to move on non free cell" << std::endl;
      throw std::exception();
    }
#endif // DEBUG

    board.make_move(c, player_id);
    this->moves.push(c);
    auto cell_id = board.to_node_id(c);

    auto adjacents = board.get_adjacents_from_id(cell_id, player_id);
    auto change_counter = connect_to(cell_id, adjacents, player_id);

    int distance_to_source;
    int distance_to_sink;
    if (player_id == 1) {
      distance_to_source = c.col;
      distance_to_sink = this->board.row_size() - 1 - c.col;
    }
    else {
      distance_to_source = c.row;
      distance_to_sink = this->board.row_size() - 1 - c.row;
    }

    auto dsu = (player_id == 1) ? &this->horizontal_board : &this->vertical_board;

    bool change_occurred = false;
    if (distance_to_source == 0) {
      change_occurred = dsu->merge(cell_id, this->SOURCE);
    }
    if (distance_to_sink == 0) {
      change_occurred = dsu->merge(cell_id, this->SINK);
    }
    
    if (change_occurred) change_counter++;

    auto change_stack = player_id == 1 ? &this->horizontal_changes_by_move
                                       : &this->vertical_changes_by_move;
    change_stack->push(change_counter);
  }

  void undo_last_move() {
    auto c = this->moves.top();
    this->moves.pop();
    auto player_id = board.get_value_at(c);

#ifdef DEBUG
    if (player_id == 0) {
      std::cerr << "Undoing move on empty cell" << std::endl;
      throw std::exception();
    }
#endif // DEBUG

    auto change_stack = (player_id == 1) ? &this->horizontal_changes_by_move
                                         : &this->vertical_changes_by_move;

    auto dsu =
        (player_id == 1) ? &this->horizontal_board : &this->vertical_board;

#ifdef DEBUG
    if (change_stack->empty()) {
      std::cerr << "Empty change stack" << std::endl;
      throw std::exception();
    }
#endif // DEBUG

    auto change_counter = change_stack->top();
    change_stack->pop();

    for (int i = 0; i < change_counter; i++) {
      dsu->undo();
    }

    board.undo_move(c);
  }

  std::vector<int> get_valid_moves() {
    return this->board.get_valid_moves();
  }

  Cell from_node_id(int node_id) {
    return this->board.from_node_id(node_id);
  }

  short get_winner() {
    if (this->horizontal_board.setOf(this->SOURCE) == this->horizontal_board.setOf(this->SINK)) {
      return 1;
    }
    if (this->vertical_board.setOf(this->SOURCE) == this->vertical_board.setOf(this->SINK)) {
      return 2;
    }

    return 0;
  }
};
