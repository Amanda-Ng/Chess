# Networked Chess-Playing Program

## Overview:
This repository contains the implementation of a rudimentary, networked chess-playing program. A server is started by one player, and a second player connects as a client. Players take turns making moves until one player forfeits or the game ends. The game state is synchronized between the client and server via message exchanges that describe each player's move.

## Features
- ```void initialize_game(ChessGame *game)```:
Initializes the chessboard and game state to represent the starting positions of the pieces. The chessboard is stored in a ChessGame struct, with rows and columns indexed starting from 0.
- ```bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game)```:
Central function that determines which specific piece-validation function to call, ensuring valid moves.
- ```int parse_move(const char *str, ChessMove *move)```: 
Parses a string representing a chess move (e.g., "e2e4") and updates the ChessMove struct with the move's details. Handles special cases like pawn promotion (e.g., "e7e8q" for a pawn promoting to a queen).
- ```int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move)```:
Executes the parsed move on the game’s chessboard. Checks for discrepancies in the game state and ensures valid moves based on the chess rules.
- ```void chessboard_to_fen(char fen[], ChessGame *game)```:
Generates a Forsyth-Edwards Notation (FEN) string to represent the current state of the chessboard.
- ```void fen_to_chessboard(const char *fen, ChessGame *game)```:
Parses a FEN string to reconstruct the state of the chessboard. Ensures the chessboard is correctly set up based on the FEN string.
- ```int save_game(ChessGame *game, const char *username, const char *db_filename)```:
Saves the current game state to a file, including the player's username and the FEN string of the game.
Multiple saves for the same user are allowed without overwriting previous saves.
- ```int load_game(ChessGame *game, const char *username, char *db_filename, int save_number)```:
Loads a saved game state for a given username and save number.
If no saved game is found for the specified user and save number, the function returns an error.
