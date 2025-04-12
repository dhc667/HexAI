# MC Hex

## Setup

The logic of the AI player is written in C++ (that's a `string`), a shared library with the implementation is provided in `wrapper/libgetmove.so`, along with the shell script used to build it, in `build.sh`, `wrapper/wrapper.py` provides an interface to call this library, `wrapper/player.py` implements the `Player` class. `wrapper/libgetmove.so.sum` is a b2sum of the .so file, to check it's the same as the one uploaded.

The time the player has for each move is a parameter, given in milliseconds. It is assumed this will be greater than 10ms.

```python
## wrapper/wrapper.py

def call_get_move(board, board_size, player_id, milliseconds):
    pass
```

## TL;DR

This is an implementation for an intelligent Hex player, using what amounts to pure Monte Carlo Tree Search (MCTS) to pick moves.

This repository has two similar implementations of MCTS, `monte_carlo`, and `monte_carlo_dsu`
- The first uses a relatively expensive function to check if, given a game state, a decisive move can be made, which can allow for early stopping in playouts, but significantly increases their cost. It was found to not be cost effective.
- The second uses an inexpensive method of defining whether there is a winner in a game state, this will cause it to explore more moves, but the time complexity differences are stark.

Upon making them play against each other, the second one was the victor more often than not, so that one was picked as the default.

Other options were explored, including a MiniMax-based player, however, the exploitation of the time constraints was quite difficult with this approach, the fact that MCTS is an "anytime" algorithm was one of the main reasons MCTS was chosen. Personally, it was also rather interesting.

## Strategy explanation

For both implementations, the idea is simple, the program loops, `visiting` the root node of a tree structure, until it runs out of time.

Each node represents a sequence of moves starting from the original game state with which the `get_move` function was called (not precisely **game states** per se, as the same game state can be reached through different sets of moves).

The [MCTS algorithm](https://en.wikipedia.org/wiki/Monte_Carlo_tree_search) is followed with little deviation, for the first implementation, upon each visit, we check if a move can be made such that the edge nodes relevant to the current player can be connected, a **decisive move**, if this is the case, sibling states are not explored, assuming a player wouldn't either, the second implementation sacrifices this early stopping for a $O(log n)$ check for whether the current state is a terminal state (i.e., there is a connection of the edge node for some player) (Of course, for both implementations, the first check the player does is checking if there is such a decisive move). Empirically, the second implementation was superior, as it was able to explore much more of the tree.

One of the most important ideas of MCTS is that little to no domain knowledge is needed to create a strong player, however, especially for larger boards, even when exploiting time constraints to the maximum, a player based on an evaluation function could be a better approach, given that the branching factor of HEX is quite large.
