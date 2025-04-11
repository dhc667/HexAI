#pragma once

#include "../models/board.hpp"
#include "../utils/dijkstra_board.hpp"
#include "tree.hpp"
#include <algorithm>
#include <chrono>
#include <set>
#include <string>
#include <vector>

// #define DEBUG 1

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

class MCTSearch_Tree {
private:
  Board board;
  MCTreeNode *root;
  double c;
  std::set<Cell> valid_moves;
  std::map<std::string, bool> decisive_moves_cache;
  short player_id;
  bool player_score_switching;

public:
  MCTSearch_Tree(Board &board, short player_id,
                 double c, bool player_score_switching)
      : board(board), c(c), player_id(player_id),
        player_score_switching(player_score_switching), root(nullptr) {
    this->valid_moves = std::set<Cell>();
    this->decisive_moves_cache = std::map<std::string, bool>();
  }

  void fill_tree(unsigned int milliseconds,
                 bool player_score_switching = true) {
    auto start_time = std::chrono::steady_clock::now();
    auto end_time = start_time + std::chrono::milliseconds(milliseconds) - std::chrono::milliseconds(2);
    auto valid_moves = this->board.get_valid_moves();

    for (auto m : valid_moves) {
      this->valid_moves.insert(board.from_node_id(m));
    }

    if (this->root == nullptr) {
      this->root = MCTreeNode::new_tree_node(this->valid_moves, 0);
    }


    while (std::chrono::steady_clock::now() < end_time) {
      // std::cerr << "Filling tree" << std::endl;
      visit(root, this->player_id);

      #ifdef DEBUG
      std::cerr << "Root: ni: " << root->data.ni << ", si: " << root->data.si << std::endl;
      #endif // DEBUG
    }
  }

  std::vector<std::pair<Cell, NodeData>> get_moves() {
    std::vector<std::pair<Cell, NodeData>> answ;

    for (auto child : this->root->children) {
      if (child.second == nullptr) continue;

      answ.push_back({child.first, child.second->data});
    }

    std::random_shuffle(answ.begin(), answ.end());

    return answ;
  }

  short visit(MCTreeNode *node, short player_id) {
    node->data.ni += 1;

    if (node->decisive_move) {
      node->data.si += node_score(player_id, player_id);
      return player_id;
    }
    auto enemy_id = enemy_of(player_id);

    if (!node->unexplored_moves.empty()) {
      // std::cerr << "Unexplored moves" << std::endl;
      auto random_move = pick_random_move(node->unexplored_moves);
      node->unexplored_moves.erase(random_move);

      this->valid_moves.erase(random_move);
      // std::cerr << "Making move: " << random_move.row << ", " << random_move.col << std::endl;
      this->board.make_move(random_move, player_id);

      auto new_child = MCTreeNode::new_tree_node(this->valid_moves);
      node->children[random_move] = new_child;
      auto winner_id = playout(new_child, enemy_id);

      this->valid_moves.insert(random_move);
      this->board.undo_move(random_move);

      node->data.si += node_score(player_id, winner_id);
      // if (winner_id != 1)
        // std::cerr << "winner " << winner_id << std::endl;
      return winner_id;
    }
    else {
      // std::cerr << "Explored moves" << std::endl;
      auto move = select_move(node);

      this->valid_moves.erase(move);
      // std::cerr << "Making move: " << move.row << ", " << move.col << std::endl;
      this->board.make_move(move, player_id);

      auto winner_id = visit(node->children[move], enemy_id);

      this->valid_moves.insert(move);
      this->board.undo_move(move);

      node->data.si += node_score(player_id, winner_id);
      // if (winner_id != 1)
        // std::cerr << "winner " << winner_id << std::endl;
      return winner_id;
    }
  }

  /// randomly plays out until game end, returns winner id
  short playout(MCTreeNode *start, short player_id) {
    // std::cerr << "Playout" << std::endl;
    std::vector<Cell> moves_made;
    auto current_player = player_id;

    while (!decisive_move_can_be_made(current_player)) {
      if (this->valid_moves.empty()) {
        std::cerr << "No valid moves and decisive move cannot be made" << std::endl;
        std::cerr << board.to_string() << std::endl;
        for (auto it = moves_made.end() - 1; it != moves_made.begin(); it--) {
          std::cerr << it->row << ", " << it->col << std::endl;
          board.undo_move(*it);
          std::cerr << board.to_string() << std::endl;
        }
      }
      auto random_move = pick_random_move(this->valid_moves);
      board.make_move(random_move, current_player);
      this->valid_moves.erase(random_move);
      moves_made.push_back(random_move);
      current_player = enemy_of(current_player);
      // std::cerr << "Making move: " << random_move.row << ", " << random_move.col << std::endl;
    }
    // if (current_player == 2)
      // std::cerr << "Decisive move can be made" << std::endl;

    for (auto m : moves_made) {
      this->board.undo_move(m);
      this->valid_moves.insert(m);
    }

    auto score = node_score(player_id, current_player);

    start->data.ni = 1;
    start->data.si += score;

    if (moves_made.empty()) {
      start->decisive_move = true;
    }

    // std::cerr << "Playout end" << std::endl;

    return current_player;
  }

  static short enemy_of(short player_id) { 
    // std::cerr << "Enemy of: " << player_id << std::endl;
    return player_id == 1 ? 2 : 1; 
  }

  short node_score(short current_player, short winner_id) {
    if (!player_score_switching)
      return winner_id == this->player_id ? 1 : 0;
    return current_player == winner_id ? 1 : 0;
  }

  bool decisive_move_can_be_made(short player_id) {
    // std::cerr << "Decisive move can be made?" << std::endl;
    auto board_hash = this->board.get_hash();
    // std::cerr << "Board hash: " << board_hash << std::endl;
    auto cached_has_decisive_move = this->decisive_moves_cache.find(board_hash);
    if (cached_has_decisive_move == this->decisive_moves_cache.end()) {
      // std::cerr << "Cache miss" << std::endl;
      auto decisive_move = get_decisive_move(this->board, player_id);
      bool decisive_move_exists = decisive_move.col != -1;

      this->decisive_moves_cache[board_hash] = decisive_move_exists;
      // std::cerr << "Decisive move exists: " << decisive_move_exists << std::endl;
      return decisive_move_exists;
    }
    // std::cerr << "Cache hit" << std::endl;

    return cached_has_decisive_move->second;
  }

  static Cell pick_random_move(const std::set<Cell> &valid_moves) {
    auto it = valid_moves.begin();
    std::advance(it, rand() % valid_moves.size());
    return *it;
  }

  Cell select_move(MCTreeNode *node) {
    Cell best_move;
    double best_value = std::numeric_limits<double>::lowest();

    for (const auto &child : node->children) {
      // std::cerr << "Child: " << child.first.row << ", " << child.first.col
      //           << std::endl;
      double uct_value = child.second->get_uct_value(node->data.ni, this->c);
      if (uct_value > best_value) {
        best_value = uct_value;
        best_move = child.first;
      }
    }

    return best_move;
  }
};
