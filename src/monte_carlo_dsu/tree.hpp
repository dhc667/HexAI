#pragma once

#include "../models/cell.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <set>

typedef struct NodeData {
  unsigned int si;
  unsigned int ni;


  double h;
} NodeData;

class MCTreeNode {
public:
  NodeData data;
  std::map<Cell, MCTreeNode *> children;
  std::set<Cell> unexplored_moves;
  short terminal_winner;

  MCTreeNode() {
    children = std::map<Cell, MCTreeNode *>();
    unexplored_moves = std::set<Cell>();
  }

  ~MCTreeNode() {
    for (auto kvpair : this->children) {
      delete kvpair.second;
    }
    children.clear();
    unexplored_moves.clear();
  }

  static MCTreeNode *new_tree_node(const std::set<Cell> &valid_moves,
                                   double heuristic = 0) {
    auto node = new MCTreeNode();
    node->data.si = 0;
    node->data.ni = 0;
    node->data.h = heuristic;
    node->terminal_winner = 0;

    node->unexplored_moves.insert(valid_moves.begin(), valid_moves.end());

    return node;
  }


  bool has_been_explored(Cell c) {
    return this->children.find(c) != this->children.end();
  }

  double get_uct_value(int parent_ni, double c, double h_weight = 0) {
    if (this->data.ni == 0) {
      return std::numeric_limits<double>::max();
    }

    double uct = (double)this->data.si / this->data.ni +
                 c * sqrt(log(parent_ni) / this->data.ni);

    return uct + h_weight * data.h;
  }
};
