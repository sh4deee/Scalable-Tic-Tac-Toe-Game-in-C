/*
 * Scalable Tic Tac Toe Game in C
 *
 * Author: Abdelrahman Khalid
 * Date: July 2025
 */

#include <stdio.h>
#include <stdlib.h>

// --- Function Definitions ---

void get_input(int *x) {
    while (scanf("%d", x) != 1) {
        while (getchar() != '\n');  // Clear invalid input
        printf("Invalid input, please enter a number: ");
    }
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_board(int *board, int DIM) {
    int size = DIM * DIM;
    int index = 0;

    while (index < size) {
        char *symbol = board[index] == 1 ? " X " :
                       board[index] == 2 ? " O " : "   ";

        printf("%s", symbol);

        if ((index + 1) % DIM != 0) {
            printf("|");
        }

        if ((index + 1) % DIM == 0) {
            printf("\n");
            if ((index + 1) != size) {
                for (int j = 0; j < DIM; j++) {
                    printf("---");
                    if (j != DIM - 1)
                        printf("+");
                }
                printf("\n");
            }
        }

        index++;
    }
}

void fill_win_array(int **winArray, int DIM) {
    int count = 0;

    // Rows
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            winArray[count][j] = i * DIM + j;
        }
        count++;
    }

    // Columns
    for (int j = 0; j < DIM; ++j) {
        for (int i = 0; i < DIM; ++i) {
            winArray[count][i] = i * DIM + j;
        }
        count++;
    }

    // Main diagonal
    for (int i = 0; i < DIM; ++i) {
        winArray[count][i] = i * DIM + i;
    }
    count++;

    // Anti-diagonal
    for (int i = 0; i < DIM; ++i) {
        winArray[count][i] = i * DIM + (DIM - 1 - i);
    }
    count++;
}

int check_win(int *board, int **winArray, int winCount, int DIM, int currentPlayer) {
    for (int i = 0; i < winCount; ++i) {
        int win = 1;
        for (int j = 0; j < DIM; ++j) {
            if (board[winArray[i][j]] != currentPlayer) {
                win = 0;
                break;
            }
        }
        if (win) return 1;
    }
    return 0;
}

int is_draw(int *board, int size) {
    for (int i = 0; i < size; ++i) {
        if (board[i] == 0) return 0;
    }
    return 1;
}

int check_play(int *board, int size, int playerMove) {
    if (playerMove < 0 || playerMove >= size) return 0;
    if (board[playerMove] != 0) return 0;
    return 1;
}

int main() {
    int DIM;

    // Get board size
    do {
        printf("Enter board size (3-10): ");
        get_input(&DIM);
        if (DIM < 3 || DIM > 10) {
            printf("Invalid size. Please enter a number between 3 and 10.\n");
        }
    } while (DIM < 3 || DIM > 10);

    int SIZE = DIM * DIM;

    // Allocate board
    int *board = calloc(SIZE, sizeof(int));
    if (!board) {
        fprintf(stderr, "Memory allocation failed for board\n");
        return EXIT_FAILURE;
    }

    // Allocate win array
    int winCount = 2 * DIM + 2;
    int **winArray = malloc(winCount * sizeof(int *));
    if (!winArray) {
        fprintf(stderr, "Memory allocation failed for winArray\n");
        free(board);
        return EXIT_FAILURE;
    }
    for (int i = 0; i < winCount; ++i) {
        winArray[i] = malloc(DIM * sizeof(int));
        if (!winArray[i]) {
            fprintf(stderr, "Memory allocation failed for winArray[%d]\n", i);
            for (int k = 0; k < i; k++)
                free(winArray[k]);
            free(winArray);
            free(board);
            return EXIT_FAILURE;
        }
    }

    fill_win_array(winArray, DIM);

    int currentPlayer = 1;

    while (1) {
        clear_screen();
        printf("\n\tTic Tac Toe\n\nPlayer 1 (X) - Player 2 (O)\n\n");
        print_board(board, DIM);

        printf("\nPlayer %d, enter your choice (1-%d): ", currentPlayer, SIZE);
        int playerMove;
        get_input(&playerMove);
        playerMove -= 1;  // 1-based to 0-based index

        if (check_play(board, SIZE, playerMove)) {
            board[playerMove] = currentPlayer;
        } else {
            printf("Invalid move! Press Enter to try again...\n");
            getchar(); getchar();
            continue;
        }

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
