/*
 * Scalable Tic Tac Toe Game in C
 *
 * Author: Abdelrahman Khalid
 * Date: July 23, 2025
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// --- Function Definitions ---

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
                    if (j != DIM -1)
                        printf("+"); // plus signs between columns
                }
                printf("\n");
            }
        }
    }
}

int get_input() {
    int input;
    scanf("%d", &input);
    return input - 1;  // So user enters 1–9 instead of 0–8
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


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


int check_win(int board[], int **winArray, int winCount, int dim, int currentPlayer) {
    for (int i = 0; i < 2 * dim + 2; ++i) {
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
int check_play(int board[], int size, int playerMove) {
    if (playerMove < 0 || playerMove >= size) return 0;
    if (board[playerMove] != 0) return 0;
    return 1;
}

int main() {
    int DIM;
    printf("Enter board size (e.g., 3 for 3x3): ");
    DIM = get_input() + 1;
    int SIZE = DIM * DIM;

    // Allocate board dynamically
    int *board = calloc(SIZE, sizeof(int));

    // Total win combinations = 2 * DIM + 2
    int winCount = 2 * DIM + 2;

    // Allocate 2D win array
    int **winArray = malloc(winCount * sizeof(int *));
    for (int i = 0; i < winCount; i++) {
        winArray[i] = malloc(DIM * sizeof(int));
    }

    // Fill winning combinations
    fill_win_array(winArray, DIM);

    int currentPlayer = 1;

    while (1) {
        clear_screen();
        printf("\n\tTic Tac Toe\n\nPlayer 1 (X) - Player 2 (O)\n\n");
        print_board(board, DIM);

        printf("\nplayer %d enter your choice: ", currentPlayer);

        int playerMove = get_input();

        if (check_play(board, SIZE, playerMove)) {
            board[playerMove] = currentPlayer;
        } else {
            continue;
        }

        // Check if current player won BEFORE switching
        if (check_win(board, winArray, winCount, DIM, currentPlayer)) {
            clear_screen();
            printf("\n\tTic Tac Toe\n\nPlayer 1 (X) - Player 2 (O)\n\n");
            print_board(board, DIM);
            printf("\nplayer %d won!\n", currentPlayer);
            break;
        }

        // Now switch turn
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

