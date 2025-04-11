#pragma once

#include "../models/board.hpp"
#include <climits>
#include <queue>
#include <set>
#include <vector>

class CompareBySecond {
public:
  bool operator()(const std::pair<int, int> &a, const std::pair<int, int> &b) {
    return a.second > b.second;
  }
};

void source_sink_dijkstra_until_k(const Board &board, short player_id, int k,
                                  bool from_source, std::vector<int> &d) {
  d.assign(board.row_size() * board.row_size(), INT_MAX);

  auto start_nodes = board.get_source_sink_adjacents(player_id, from_source);
  auto goal_nodes = board.get_source_sink_adjacents(player_id, !from_source);

  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                      CompareBySecond>
      pq; // node id with cost

  std::set<int> visited;

  for (auto node : start_nodes) {
    d[node] = board.is_self_occupied_id(node, player_id) ? 0 : 1;
    pq.push({node, d[node]});
  }

  while (!pq.empty()) {
    auto node_data = pq.top();
    auto node = node_data.first;
    pq.pop();

    if (d[node] > k) {
      break;
    }

    if (visited.find(node) != visited.end()) {
      continue;
    }

    visited.insert(node);
    int self_occupy_cost = board.is_self_occupied_id(node, player_id) ? 0 : 1;

    auto adjacents = board.get_adjacents_from_id(node, player_id);
    for (auto adjacent : adjacents) {
      if (visited.find(adjacent) == visited.end()) {
        auto cost = board.is_self_occupied_id(adjacent, player_id) ? 0 : 1;
        cost += self_occupy_cost;

        if (d[node] + cost < d[adjacent]) {
          d[adjacent] = d[node] + cost;
          pq.push({adjacent, d[adjacent]});
        }
      }
    }
  }
}

void source_sink_distances_until_2(const Board &board, short player_id,
                                   std::vector<int> &source_d,
                                   std::vector<int> &sink_d) {
  source_sink_dijkstra_until_k(board, player_id, 2, true, source_d);
  source_sink_dijkstra_until_k(board, player_id, 2, false, sink_d);
}

Cell get_decisive_move(const Board &board, short player_id) {
  static std::vector<int> source_d;
  static std::vector<int> sink_d;

  source_sink_distances_until_2(board, player_id, source_d, sink_d);
  for (int i = 0; i < board.row_size(); i++) {
    for (int j = 0; j < board.row_size(); j++) {
      auto cell = board.to_node_id({i, j});
      if (!board.is_free_id(cell)) {
        continue;
      }
      if (source_d[cell] <= 2 && sink_d[cell] <= 2) {
        return {i, j};
      }
    }
  }

  return {-1, -1};
}

bool player_won(const Board &board, short player_id) {
  static std::vector<int> d;

  source_sink_dijkstra_until_k(board, player_id, 0, true, d);

  for (auto sink_adj : board.get_source_sink_adjacents(player_id, false)) {
    auto cell = board.from_node_id(sink_adj);
    if (d[sink_adj] == 0) {
      return true;
    }
  }

  return false;
}

bool _game_over(const Board &board) {
  if (player_won(board, 1)) return true;
  if (player_won(board, 2)) return true;

  return false;
}
