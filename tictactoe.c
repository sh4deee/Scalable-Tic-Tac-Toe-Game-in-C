/*
 * Scalable Tic Tac Toe Game in C
 *
 * Author: Abdelrahman Khalid
 * Date: July 2025
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// --- Function Definitions ---

int get_input() {
    int input;
    while (scanf("%d", &input) != 1) {
        while (getchar() != '\n');  // Clear invalid input
        printf("Invalid input, please enter a number: ");
    }
    return input;
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_board(int board[], int DIM) {
    int size = DIM * DIM;

    for (int i = 0; i < size; i++) {
        // Print symbol with spaces (3 chars wide)
        char *symbol = board[i] == 1 ? " X " :
                       board[i] == 2 ? " O " : "   ";

        printf("%s", symbol);

        // Print vertical bar if not last column
        if ((i + 1) % DIM != 0) {
            printf("|");
        }

        // End of row
        if ((i + 1) % DIM == 0) {
            printf("\n");

            // Print horizontal line between rows, but not after last row
            if ((i + 1) != size) {
                for (int j = 0; j < DIM; j++) {
                    printf("---");    // 3 dashes for each cell width
                    if (j != DIM - 1)
                        printf("+"); // plus signs between columns
                }
                printf("\n");
            }
        }
    }
}

// Fill the winArray with all winning combinations (rows, columns, diagonals)
void fill_win_array(int **winArray, int dim) {
    int count = 0;

    // Fill all rows
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            winArray[count][j] = i * dim + j;
        }
        count++;
    }

    // Fill all columns
    for (int j = 0; j < dim; ++j) {
        for (int i = 0; i < dim; ++i) {
            winArray[count][i] = i * dim + j;
        }
        count++;
    }

    // Fill main diagonal (top-left to bottom-right)
    for (int i = 0; i < dim; ++i) {
        winArray[count][i] = i * dim + i;
    }
    count++;

    // Fill anti-diagonal (top-right to bottom-left)
    for (int i = 0; i < dim; ++i) {
        winArray[count][i] = i * dim + (dim - 1 - i);
    }
    count++;
}

// Check if currentPlayer has a winning line
int check_win(int board[], int **winArray, int winCount, int dim, int currentPlayer) {
    for (int i = 0; i < winCount; ++i) {
        bool win = true;
        for (int j = 0; j < dim; ++j) {
            if (board[winArray[i][j]] != currentPlayer) {
                win = false;
                break;
            }
        }
        if (win) return 1;  // Player wins
    }
    return 0; // No win
}

// Check if the board is full without any winner
bool is_draw(int board[], int size) {
    for (int i = 0; i < size; ++i) {
        if (board[i] == 0) return false;  // Empty cell found, not a draw
    }
    return true;  // No empty cells, draw
}

// Check if a move is valid (inside range and empty)
bool check_play(int board[], int size, int playerMove) {
    if (playerMove < 0 || playerMove >= size) return false;
    if (board[playerMove] != 0) return false;
    return true;
}

int main() {
    int DIM;

    // Get board size with validation (3 to 10)
    do {
        printf("Enter board size (3-10): ");
        DIM = get_input();
        if (DIM < 3 || DIM > 10) {
            printf("Invalid size. Please enter a number between 3 and 10.\n");
        }
    } while (DIM < 3 || DIM > 10);

    int SIZE = DIM * DIM;

    // Allocate board dynamically and check for success
    int *board = calloc(SIZE, sizeof(int));
    if (!board) {
        fprintf(stderr, "Memory allocation failed for board\n");
        exit(EXIT_FAILURE);
    }

    // Total winning combinations = 2 * DIM + 2
    int winCount = 2 * DIM + 2;

    // Allocate 2D win array and check allocation
    int **winArray = malloc(winCount * sizeof(int *));
    if (!winArray) {
        fprintf(stderr, "Memory allocation failed for winArray\n");
        free(board);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < winCount; i++) {
        winArray[i] = malloc(DIM * sizeof(int));
        if (!winArray[i]) {
            fprintf(stderr, "Memory allocation failed for winArray[%d]\n", i);
            // Free previously allocated rows
            for (int k = 0; k < i; k++)
                free(winArray[k]);
            free(winArray);
            free(board);
            exit(EXIT_FAILURE);
        }
    }

    fill_win_array(winArray, DIM);

    int currentPlayer = 1;

    while (1) {
        clear_screen();
        printf("\n\tTic Tac Toe\n\nPlayer 1 (X) - Player 2 (O)\n\n");
        print_board(board, DIM);

        printf("\nPlayer %d, enter your choice (1-%d): ", currentPlayer, SIZE);
        int playerMove = get_input() - 1;  // Convert to zero-based index

        if (check_play(board, SIZE, playerMove)) {
            board[playerMove] = currentPlayer;
        } else continue;

        if (check_win(board, winArray, winCount, DIM, currentPlayer)) {
            clear_screen();
            printf("\n\tTic Tac Toe\n\nPlayer 1 (X) - Player 2 (O)\n\n");
            print_board(board, DIM);
            printf("\nPlayer %d won!\n", currentPlayer);
            break;
        }

        if (is_draw(board, SIZE)) {
            clear_screen();
            printf("\n\tTic Tac Toe\n\nPlayer 1 (X) - Player 2 (O)\n\n");
            print_board(board, DIM);
            printf("\nIt's a draw!\n");
            break;
        }

        // Switch turn
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    // Free memory
    for (int i = 0; i < winCount; i++) {
        free(winArray[i]);
    }
    free(winArray);
    free(board);

    return 0;
}
