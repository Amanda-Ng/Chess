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
    // (void)piece;
    // (void)src_row;
    // (void)src_col;
    // (void)dest_row;
    // (void)dest_col;
    // (void)game;
    // return false;
    int player = (piece >= 'a') ? BLACK_PLAYER : WHITE_PLAYER;
    int direction = (player == WHITE_PLAYER) ? -1 : 1;

    // Check if destination square is within the board limits
    if (dest_row < 0 || dest_row > 7 || dest_col < 0 || dest_col > 7) {
        return false;
    }

    // Check if pawn is moving forward
    if ((dest_row - src_row) * direction <= 0) {
        return false;
    }

    // Check if pawn is moving two squares forward from its starting position
    if (src_row == (player == WHITE_PLAYER ? 6 : 1) && dest_row - src_row == 2 * direction) {
        // Check if the path is clear (no piece blocking)
        if (game->chessboard[src_row + direction][src_col] != '.') {
            return false;
        }
    }

    // Check if pawn is moving one square forward
    if ((dest_row - src_row == 1 * direction) && (abs(dest_col - src_col) == 0)) {
        // Check if the path is clear (no piece blocking)
        if (game->chessboard[dest_row][dest_col] != '.') {
            return false;
        }
    }

    // Check if pawn is capturing diagonally
    if (dest_col != src_col && game->chessboard[dest_row][dest_col] != '.') {
        // Check if capturing diagonally
        if (abs(dest_col - src_col) == 1 && abs(dest_row - src_row) == 1) {
            return true;
        } else {
            return false;
        }
    } else if (dest_col != src_col && game->chessboard[dest_row][dest_col] == '.'){
        return false;
    }

    return true;
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    // (void)src_row;
    // (void)src_col;
    // (void)dest_row;
    // (void)dest_col;
    // (void)game;
    // return false;
    // Check if the move is horizontal or vertical
    if (src_row != dest_row && src_col != dest_col) {
        return false; // Not moving horizontally or vertically
    }
    
    // Check if there are no pieces blocking the path between source and destination
    if (src_row == dest_row) {
        // Moving horizontally
        int delta_col = (dest_col > src_col) ? 1 : -1;
        int col = src_col + delta_col;
        while (col != dest_col) {
            if (game->chessboard[src_row][col] != '.') {
                return false; // Piece blocking the path
            }
            col += delta_col;
        }
    } else {
        // Moving vertically
        int delta_row = (dest_row > src_row) ? 1 : -1;
        int row = src_row + delta_row;
        while (row != dest_row) {
            if (game->chessboard[row][src_col] != '.') {
                return false; // Piece blocking the path
            }
            row += delta_row;
        }
    }
    
    return true;
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    // (void)src_row;
    // (void)src_col;
    // (void)dest_row;
    // (void)dest_col;
    // return false;
    // Check if the move is in L-shape (two squares vertically and one square horizontally, or vice versa)
    int row_diff = abs(dest_row - src_row);
    int col_diff = abs(dest_col - src_col);
    return ((row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2));
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    // (void)src_row;
    // (void)src_col;
    // (void)dest_row;
    // (void)dest_col;
    // (void)game;
    // return false;
    // Check if the move is diagonal
    int row_diff = abs(dest_row - src_row);
    int col_diff = abs(dest_col - src_col);
    if (row_diff != col_diff) {
        return false; // Not moving diagonally
    }
    
    // Check if there are no pieces blocking the path between source and destination
    int delta_row = (dest_row > src_row) ? 1 : -1;
    int delta_col = (dest_col > src_col) ? 1 : -1;
    int row = src_row + delta_row;
    int col = src_col + delta_col;
    while (row != dest_row && col != dest_col) {
        if (game->chessboard[row][col] != '.') {
            return false; // Piece blocking the path
        }
        row += delta_row;
        col += delta_col;
    }
    
    return true;
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    // (void)src_row;
    // (void)src_col;
    // (void)dest_row;
    // (void)dest_col;
    // (void)game;
    // return false;
    // Check if the move is horizontal, vertical, or diagonal
    int row_diff = abs(dest_row - src_row);
    int col_diff = abs(dest_col - src_col);
    
    // Check if the move is horizontal, vertical, or diagonal
    if (src_row != dest_row && src_col != dest_col && row_diff != col_diff) {
        return false; // Not moving horizontally, vertically, or diagonally
    }
    
    // Check if there are no pieces blocking the path between source and destination
    if (src_row == dest_row) {
        // Moving horizontally
        int delta_col = (dest_col > src_col) ? 1 : -1;
        int col = src_col + delta_col;
        while (col != dest_col) {
            if (game->chessboard[src_row][col] != '.') {
                return false; // Piece blocking the path
            }
            col += delta_col;
        }
    } else if (src_col == dest_col) {
        // Moving vertically
        int delta_row = (dest_row > src_row) ? 1 : -1;
        int row = src_row + delta_row;
        while (row != dest_row) {
            if (game->chessboard[row][src_col] != '.') {
                return false; // Piece blocking the path
            }
            row += delta_row;
        }
    } else {
        // Moving diagonally
        int delta_row = (dest_row > src_row) ? 1 : -1;
        int delta_col = (dest_col > src_col) ? 1 : -1;
        int row = src_row + delta_row;
        int col = src_col + delta_col;
        while (row != dest_row && col != dest_col) {
            if (game->chessboard[row][col] != '.') {
                return false; // Piece blocking the path
            }
            row += delta_row;
            col += delta_col;
        }
    }
    
    return true;
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    // (void)src_row;
    // (void)src_col;
    // (void)dest_row;
    // (void)dest_col;
    // return false;
    // Check if the move is one square in any direction (horizontally, vertically, or diagonally)
    int row_diff = abs(dest_row - src_row);
    int col_diff = abs(dest_col - src_col);
    
    // Check if the move is one square in any direction
    return (row_diff <= 1 && col_diff <= 1);
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    // (void)piece;
    // (void)src_row;
    // (void)src_col;
    // (void)dest_row;
    // (void)dest_col;
    // (void)game;
    // return false;
        // Check if there is a piece at the source location
    if (game->chessboard[src_row][src_col] != piece) {
        return false; // No piece at the given source location
    }
    
    // Call the appropriate is_valid function based on the piece
    switch (piece) {
        case 'p':
        case 'P':
            return is_valid_pawn_move(piece, src_row, src_col, dest_row, dest_col, game);
        case 'r':
        case 'R':
            return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game);
        case 'n':
        case 'N':
            return is_valid_knight_move(src_row, src_col, dest_row, dest_col);
        case 'b':
        case 'B':
            return is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
        case 'q':
        case 'Q':
            return is_valid_queen_move(src_row, src_col, dest_row, dest_col, game);
        case 'k':
        case 'K':
            return is_valid_king_move(src_row, src_col, dest_row, dest_col);
        default:
            return false; // Unknown piece type
    }
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

int parse_move(const char *str, ChessMove *move) {
    // (void)move;
    // (void)parsed_move;
    // return -999;
    // Check if the length of the move string is valid
    size_t len = strlen(str);
    if ((len != 4 && len != 5 )) {
        return PARSE_MOVE_INVALID_FORMAT;
    }
    
    // Check if the row letter is in the range 'a' through 'h'
    if (str[0] < 'a' || str[0] > 'h' || str[2] < 'a' || str[2] > 'h') {
        return PARSE_MOVE_INVALID_FORMAT;
    }
    
    // Check if the column number is in the range '1' through '8'
    if (str[1] < '1' || str[1] > '8' || str[3] < '1' || str[3] > '8') {
        return PARSE_MOVE_OUT_OF_BOUNDS;
    }
    
    // Check if the destination row is appropriate for pawn promotion (if applicable)
    if (len == 5 && ((str[3] != '8') && (str[3] != '1'))) {
        return PARSE_MOVE_INVALID_DESTINATION;
    }
    
    // Check if the promotion piece is one of the valid options ('q', 'r', 'b', 'n')
    if (len == 5 && str[4] != 'q' && str[4] != 'r' && str[4] != 'b' && str[4] != 'n') {
        return PARSE_MOVE_INVALID_PROMOTION;
    }
    
    // Copy the start and end squares into the move structure
    strncpy(move->startSquare, str, 2);
    move->startSquare[2] = '\0';
    strncpy(move->endSquare, str + 2, 2);
    move->endSquare[2] = '\0';
    if (len == 5) {
        strncat(move->endSquare, &str[4], 1);
        move->endSquare[3] = '\0';
    }
    
    return 0; // Successful parsing of the move string
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    // (void)game;
    // (void)move;
    // (void)is_client;
    // (void)validate_move;
    // return -999;

    // Extract source and destination rows and columns from move
    int src_row = '8' - move->startSquare[1];
    int src_col = move->startSquare[0] - 'a';
    int dest_row = '8' - move->endSquare[1];
    int dest_col = move->endSquare[0] - 'a';

    // Error checks if validation is needed
    if (validate_move) {
        // Check if it's the correct player's turn
        if ((is_client && game->currentPlayer != WHITE_PLAYER) ||
            (!is_client && game->currentPlayer != BLACK_PLAYER)) {
            return MOVE_OUT_OF_TURN;
        }
        
        // Check if the source square contains a piece
        if (game->chessboard[src_row][src_col] == '.') {
            return MOVE_NOTHING;
        }
        
        // Check if the piece being moved belongs to the current player
        char piece = game->chessboard[src_row][src_col];
        if ((islower(piece) && game->currentPlayer == WHITE_PLAYER) ||
            (isupper(piece) && game->currentPlayer == BLACK_PLAYER)) {
            return MOVE_WRONG_COLOR;
        }
        
        // Check if the move results in capturing own piece
        if (islower(game->chessboard[dest_row][dest_col]) && game->currentPlayer != WHITE_PLAYER) {
            return MOVE_SUS;
        }
        if (isupper(game->chessboard[dest_row][dest_col]) && game->currentPlayer != BLACK_PLAYER) {
            return MOVE_SUS;
        }
        
        // Check if the move string has a length of 5 characters but the piece at the start square is not a pawn
        if (strlen(move->endSquare) == 3 && (piece != 'P' && piece != 'p')) {
            return MOVE_NOT_A_PAWN;
        }
        
        // Check if the move string has a length of 4 characters but the destination square is on the row for pawn promotion
        if (strlen(move->endSquare) == 2 && ((piece == 'P' && dest_row == 0) || (piece == 'p' && dest_row == 7))) {
            return MOVE_MISSING_PROMOTION;
        }
        
        // Check if the move is valid according to the rules of chess
        if (!is_valid_move(piece, src_row, src_col, dest_row, dest_col, game)) {
            return MOVE_WRONG;
        }
    }
    
    // Update game state
    // Copy the move into the moves array
    memcpy(&game->moves[game->moveCount], move, sizeof(ChessMove));
    game->moveCount++;
    
    // Update captured pieces
    if ((game->chessboard[dest_row][dest_col]) != '.') {
        game->capturedPieces[game->capturedCount] = game->chessboard[dest_row][dest_col];
        game->capturedCount++;
    }
    
    // Make the move on the chessboard
    game->chessboard[dest_row][dest_col] = game->chessboard[src_row][src_col];
    game->chessboard[src_row][src_col] = '.';
    
    // Determine current player's color
    char currentPlayer = game->currentPlayer;
    bool isWhite = (currentPlayer == WHITE_PLAYER);

    // Promote pawn if necessary
    if (strlen(move->endSquare) == 3) {
        char promotionPiece = move->endSquare[2];
        if ((isWhite && islower(promotionPiece)) || (!isWhite && isupper(promotionPiece))) {
            // Convert to uppercase if white player, or to lowercase if black player
            promotionPiece = (isWhite ? toupper(promotionPiece) : tolower(promotionPiece));
        }
        game->chessboard[dest_row][dest_col] = promotionPiece;
    }
    
    // Update currentPlayer
    game->currentPlayer = (game->currentPlayer == WHITE_PLAYER) ? BLACK_PLAYER : WHITE_PLAYER;
    
    return 0; // No error
}

int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    // (void)game;
    // (void)message;
    // (void)socketfd;
    // (void)is_client;
    // return -999;
    char command[10];
    char argument[100];
    int ret_code = COMMAND_UNKNOWN;

    // Parse command and argument from the message
    sscanf(message, "/%s %s", command, argument);

    if (strcmp(command, "move") == 0) {
        // Parse and validate the move
        ChessMove move;
        if (parse_move(argument, &move)) {
            int result = make_move(game, &move, is_client, true);
            if (result == 0) {
                // If the move is valid, send it over the socket
                send(socketfd, message, strlen(message), 0);
                ret_code = COMMAND_MOVE;
            } else {
                // If an error occurred during move execution, return COMMAND_ERROR
                ret_code = COMMAND_ERROR;
            }
        } else {
            // If the move parsing failed, return COMMAND_ERROR
            ret_code = COMMAND_ERROR;
        }
    } else if (strcmp(command, "forfeit") == 0) {
        // Send the forfeit command over the socket
        send(socketfd, message, strlen(message), 0);
        ret_code = COMMAND_FORFEIT;
    } else if (strcmp(command, "chessboard") == 0) {
        // Display the chessboard and return COMMAND_CHESSBOARD
        display_chessboard(game);
        ret_code = COMMAND_DISPLAY;
    } else if (strcmp(command, "import") == 0 && !is_client) {
        // Import FEN string and send it over the socket
        fen_to_chessboard(argument, game);
        send(socketfd, message, strlen(message), 0);
        ret_code = COMMAND_IMPORT;
    } else if (strcmp(command, "load") == 0) {
        // Parse username and game number from the argument
        char username[50];
        int game_num;
        sscanf(argument, "%s %d", username, &game_num);
        // Load the game and send a message over the socket
        if (load_game(game, username, "game_database.txt", game_num) == 0) {
            send(socketfd, message, strlen(message), 0);
            ret_code = COMMAND_LOAD;
        } else {
            // If loading failed, return COMMAND_ERROR
            ret_code = COMMAND_ERROR;
        }
    } else if (strcmp(command, "save") == 0) {
        // Save the game and return COMMAND_SAVE
        if (save_game(game, "client_username", "game_database.txt") == 0) {
            send(socketfd, message, strlen(message), 0);
            ret_code = COMMAND_SAVE;
        } else {
            // If saving failed, return COMMAND_ERROR
            ret_code = COMMAND_ERROR;
        }
    }

    return ret_code;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    // (void)game;
    // (void)message;
    // (void)socketfd;
    // (void)is_client;
    // return -999;
    char command[10];
    char argument[BUFFER_SIZE];
    int ret_code = -1;

    // Parse command and argument from the message
    sscanf(message, "/%s %s", command, argument);

    if (strcmp(command, "move") == 0) {
        // Parse and validate the move
        ChessMove move;
        if (parse_move(argument, &move)) {
            int result = make_move(game, &move, !is_client, false);
            if (result == 0) {
                ret_code = COMMAND_MOVE;
            } else {
                // If an error occurred during move execution, return COMMAND_ERROR
                ret_code = COMMAND_ERROR;
            }
        } else {
            // If the move parsing failed, return COMMAND_ERROR
            ret_code = COMMAND_ERROR;
        }
    } else if (strcmp(command, "forfeit") == 0) {
        // Close the socket and return COMMAND_FORFEIT
        close(socketfd);
        ret_code = COMMAND_FORFEIT;
    } else if (strcmp(command, "import") == 0 && !is_client) {
        // Import FEN string and return COMMAND_IMPORT
        fen_to_chessboard(argument, game);
        ret_code = COMMAND_IMPORT;
    } else if (strcmp(command, "load") == 0) {
        // Parse username and game number from the argument
        char username[50];
        int game_num;
        sscanf(argument, "%s %d", username, &game_num);
        // Load the game and return COMMAND_LOAD
        if (load_game(game, username, "game_database.txt", game_num) == 0) {
            ret_code = COMMAND_LOAD;
        } else {
            // If loading failed, return COMMAND_ERROR
            ret_code = COMMAND_ERROR;
        }
    }

    return ret_code;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    // (void)game;
    // (void)username;
    // (void)db_filename;
    // return -999;
    // Check if username is empty or contains spaces
    for (const char *ptr = username; *ptr != '\0'; ++ptr) {
        if (*ptr == ' ' || *ptr == '\t') {
            fprintf(stderr, "Error: Username cannot contain spaces or be empty.\n");
            return -1;
        }
    }

    // Generate FEN string of the current game state
    char fen[80];
    chessboard_to_fen(fen, game);
    
    // Open the database file for appending
    FILE *file = fopen(db_filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open database file.\n");
        return -1; // Error opening file
    }

    // Check if the file is empty
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    if (file_size != 0) {
        // File is not empty, append a newline
        fprintf(file, "\n");
    }

    // Write the username and FEN string to the file
    fprintf(file, "%s:%s", username, fen);
    
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
