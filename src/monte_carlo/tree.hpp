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

  // unsigned int amaf_wi;
  // unsigned int amaf_ni;

  double h;
} NodeData;

class MCTreeNode {
public:
  NodeData data;
  std::map<Cell, MCTreeNode *> children;
  std::set<Cell> unexplored_moves;
  bool decisive_move;
  // Cell decisive_cell;

  MCTreeNode() {
    // std::cerr << "Called ctor" << std::endl;
    children = std::map<Cell, MCTreeNode *>();
    unexplored_moves = std::set<Cell>();
  }

  static MCTreeNode *new_tree_node(const std::set<Cell> &valid_moves,
                                   double heuristic = 0) {
    auto node = new MCTreeNode();
    node->data.si = 0;
    node->data.ni = 0;
    // node->data.amaf_wi = 0;
    // node->data.amaf_ni = 0;
    node->data.h = heuristic;
    node->decisive_move = false;

    node->unexplored_moves.insert(valid_moves.begin(), valid_moves.end());

    return node;
  }

  static MCTreeNode *new_decisive_tree_node(/* Cell c */) {
    auto node = new MCTreeNode();
    node->data.si = 1;
    node->data.ni = 1;
    // node->data.amaf_wi = 0;
    // node->data.amaf_ni = 0;
    node->data.h = 0;
    node->decisive_move = true;
    // node->decisive_cell = c;

    return node;
  }

  bool has_been_explored(Cell c) {
    return this->children.find(c) != this->children.end();
  }

  double get_uct_value(int parent_ni, double c, double h_weight = 0) {
    // std::cerr << "UCT: ni: " << this->data.ni << ", si: " << this->data.si
    //           << ", h: " << this->data.h << ", N: " << parent_ni <<std::endl;
    if (this->data.ni == 0) {
      return std::numeric_limits<double>::max();
    }

    double uct = (double)this->data.si / this->data.ni +
                 c * sqrt(log(parent_ni) / this->data.ni);

    // std::cerr << "UCT: " << uct << std::endl;
    return uct + h_weight * data.h;
  }
};
