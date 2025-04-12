import ctypes
import os
from ctypes import POINTER, c_short

curr_dir = os.path.dirname(os.path.abspath(__file__))
lib = ctypes.CDLL(os.path.join(curr_dir, 'libgetmove.so'))

class Move(ctypes.Structure):
    _fields_ = [("row", ctypes.c_int),
                ("col", ctypes.c_int)]

lib.get_move.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_short)), ctypes.c_uint, ctypes.c_short, ctypes.c_uint]
lib.get_move.restype = Move

lib.game_over.restype = ctypes.c_bool
lib.game_over.argtypes = [
    ctypes.POINTER(ctypes.POINTER(ctypes.c_short)),  # short** _board
    ctypes.c_uint                      # unsigned int board_size
]

lib.is_winner.restype = ctypes.c_bool
lib.is_winner.argtypes = [
    ctypes.POINTER(ctypes.POINTER(ctypes.c_short)),  # short** _board
    ctypes.c_uint,                      # unsigned int board_size
    ctypes.c_short                      # short player_id
]

def call_get_move(board, board_size, player_id, milliseconds):
    board_ctypes = (ctypes.POINTER(ctypes.c_short) * board_size)()
    for i in range(board_size):
        row = (ctypes.c_short * board_size)(*board[i])
        board_ctypes[i] = ctypes.cast(row, ctypes.POINTER(ctypes.c_short))

    result = lib.get_move(board_ctypes, board_size, player_id, milliseconds)
    return result.row, result.col

def game_over(board):
    board_size = len(board)
    # Convert Python list to ctypes array
    c_board = (POINTER(c_short) * board_size)(
        *[ctypes.cast((c_short * board_size)(*row), POINTER(c_short)) for row in board]
    )
    return lib.game_over(c_board, board_size)

def is_winner(board, player_id):
    board_size = len(board)
    # Convert Python list to ctypes array
    c_board = (POINTER(c_short) * board_size)(
        *[ctypes.cast((c_short * board_size)(*row), POINTER(c_short)) for row in board]
    )
    return lib.is_winner(c_board, board_size, player_id)

def get_move(board, player_id):
    """
    Get the move from the player.
    
    Args:
        board (list[list[int]]): The game board.
        player_id (int): The ID of the player making the move.
    
    Returns:
        tuple[int, int]: The row and column of the move.
    """
    board_size = len(board)
    milliseconds = 8000
    return call_get_move(board, board_size, player_id, milliseconds)

if __name__ == "__main__":
    board_size = 5
    board = [
        [0, 0, 0, 0, 0],
        [1, 1, 1, 0, 0],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0]
    ]

    horizontal_player = True
    move = call_get_move(board, board_size, 1, 2000)
    print(f"Move: {move}")
