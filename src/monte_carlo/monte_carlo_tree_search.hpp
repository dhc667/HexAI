#pragma once

#include "../models/cell.hpp"
#include "tree.hpp"
#include "tree_search.hpp"

double get_score(NodeData data) {
  // return static_cast<float>(data.si) / data.ni;
  return data.ni;
}

Cell MCTS_get_move(short **_board, unsigned int board_size, short player_id,
                   unsigned int milliseconds, double c) {
  Board board(_board, board_size);
  auto decisive_move = get_decisive_move(board, player_id);
  if (decisive_move.col != -1) {
    return decisive_move;
  }
  MCTSearch_Tree tree(board, player_id, c, false);

  tree.fill_tree(milliseconds);
  auto moves = tree.get_moves();

  auto best_move = moves[0].first;
  auto best_score = get_score(moves[0].second);
  
  for (auto move : moves) {
    auto move_score = get_score(move.second);
    if (move_score > best_score) {
      best_score = move_score;
      best_move = move.first;
    }
  }


  return best_move;
  // return {-1, -1};
}

