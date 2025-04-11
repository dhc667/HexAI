// #include "board.hpp"
// #include <cassert>
// #include <vector>

// void node_id_test() {
//   std::vector<std::vector<short>> ex_board{{0, 1, 2}, {1, 2, 1}, {0, 0, 0}};

//   Board board(ex_board);
//   std::vector<int> expected_node_ids{0, 1, 2, 3, 4, 5, 6, 7, 8};
  

//   for (int i = 0; i < board.row_size(); i++) {
//     for (int j = 0; j < board.row_size(); j++) {
//       Cell c = {i, j};
//       int node_id = board.to_node_id(c);
//       assert(node_id == expected_node_ids[i * board.row_size() + j]);
//     }
//   }
// }

// int main() {
//   node_id_test();

//   std::cout << "All tests passed!" << std::endl;
//   return 0;
// }
