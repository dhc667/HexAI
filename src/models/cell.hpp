#pragma once
#include <string>
typedef struct Cell {
	int row;
	int col;
} Cell;

std::string cell_to_str(Cell c) {
  return std::to_string(c.row) + "," + std::to_string(c.col);
}

bool operator<(const Cell &a, const Cell &b) {
	return a.row < b.row || (a.row == b.row && a.col < b.col);
}
