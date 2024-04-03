#include "hw4.h"

void initialize_game(ChessGame *game) {
    // (void)game;
    // Initialize the chessboard
    char initial_board[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
    
    // Copy the initial board to the game's chessboard
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            game->chessboard[i][j] = initial_board[i][j];
        }
    }
    
    // Set moveCount and capturedCount to 0
    game->moveCount = 0;
    game->capturedCount = 0;
    
    // Set currentPlayer to WHITE_PLAYER
    game->currentPlayer = WHITE_PLAYER;
}

void chessboard_to_fen(char fen[], ChessGame *game) {
    // (void)fen;
    // (void)game;
    // Clear the FEN string
    memset(fen, 0, sizeof(char) * 80);
    
    // Convert the chessboard state to FEN string
    int i, j, k;
    k = 0;
    for (i = 0; i < 8; ++i) {
        int emptyCount = 0;
        for (j = 0; j < 8; ++j) {
            if (game->chessboard[i][j] == '.') {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    fen[k++] = emptyCount + '0';
                    emptyCount = 0;
                }
                fen[k++] = game->chessboard[i][j];
            }
        }
        if (emptyCount > 0) {
            fen[k++] = emptyCount + '0';
        }
        if (i < 7) {
            fen[k++] = '/';
        }
    }
    
    // Append whose turn it is
    fen[k++] = ' ';
    fen[k++] = (game->currentPlayer == WHITE_PLAYER) ? 'w' : 'b';
}

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)piece;
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    return false;
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    return false;
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)piece;
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    // (void)fen;
    // (void)game;
    // Clear the chessboard array
    memset(game->chessboard, '.', sizeof(game->chessboard));
    
    // Parse the FEN string and reconstruct the chessboard
    int i = 0, row = 0, col = 0;
    while (fen[i] != '\0') {
        if (fen[i] == '/') {
            row++;
            col = 0;
        } else if (fen[i] >= '1' && fen[i] <= '8') {
            int emptyCount = fen[i] - '0';
            for (int j = 0; j < emptyCount; ++j) {
                game->chessboard[row][col++] = '.';
            }
        } else {
            game->chessboard[row][col++] = fen[i];
        }
        i++;
    }
    
    // Determine whose turn it is
    game->currentPlayer = (fen[i - 1] == 'b') ? BLACK_PLAYER : WHITE_PLAYER;
}

int parse_move(const char *move, ChessMove *parsed_move) {
    (void)move;
    (void)parsed_move;
    return -999;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    (void)game;
    (void)move;
    (void)is_client;
    (void)validate_move;
    return -999;
}

int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    // (void)game;
    // (void)username;
    // (void)db_filename;
    // return -999;
    // Generate FEN string of the current game state
    char fen[80];
    chessboard_to_fen(fen, game);
    
    // Open the database file for appending
    FILE *file = fopen(db_filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open database file.\n");
        return -1; // Error opening file
    }
    
    // Write the username and FEN string to the file
    fprintf(file, "%s:%s\n", username, fen);
    
    // Close the file
    fclose(file);
    
    return 0; // Successful save operation
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    // (void)game;
    // (void)username;
    // (void)db_filename;
    // (void)save_number;
    // return -999;
    // Open the database file for reading
    FILE *file = fopen(db_filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open database file.\n");
        return -1; // Error opening file
    }
    
    // Read the file line by line
    char line[256];
    int count = 0;
    int found = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        // Parse username and FEN string from the line
        char user[100], fen[80];
        sscanf(line, "%99[^:]:%79s", user, fen);
        
        // Check if the username and save number match
        if (strcmp(user, username) == 0) {
            count++;
            if (count == save_number) {
                found = 1;
                // Load the game state from the FEN string
                fen_to_chessboard(fen, game);
                break;
            }
        }
    }
    
    // Close the file
    fclose(file);
    
    // Check if the game state was found
    if (!found) {
        fprintf(stderr, "Save file not found for the given username and save number.\n");
        return -1; // Game state not found
    }
    
    return 0; // Successful load operation
}

void display_chessboard(ChessGame *game) {
    printf("\nChessboard:\n");
    printf("  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", game->chessboard[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}
