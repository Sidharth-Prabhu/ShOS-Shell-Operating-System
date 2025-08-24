// tictactoe.c
#include "vga.h"
#include "klib.h"

#define BOARD_SIZE 3
#define CELL_WIDTH 5
#define CELL_HEIGHT 3
#define BOARD_START_X 30
#define BOARD_START_Y 5

// Game state
static char board[BOARD_SIZE][BOARD_SIZE];
static int current_player; // 0 for X, 1 for O
static int cursor_x, cursor_y;
static int game_over;
static int winner;

// Initialize the game
void init_game() {
    // Clear the board
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[y][x] = ' ';
        }
    }
    
    current_player = 0; // X goes first
    cursor_x = 0;
    cursor_y = 0;
    game_over = 0;
    winner = -1; // -1 = no winner, 0 = X wins, 1 = O wins, 2 = draw
}

// Draw the Tic Tac Toe board
void draw_board() {
    // Draw the board grid
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            int screen_x = BOARD_START_X + x * CELL_WIDTH;
            int screen_y = BOARD_START_Y + y * CELL_HEIGHT;
            
            // Draw cell content
            vga_putc_at(screen_x + 2, screen_y + 1, board[y][x]);
            
            // Draw grid lines
            if (x < BOARD_SIZE - 1) {
                for (int i = 0; i < CELL_HEIGHT; i++) {
                    vga_putc_at(screen_x + CELL_WIDTH, screen_y + i, '|');
                }
            }
            
            if (y < BOARD_SIZE - 1) {
                for (int i = 0; i < CELL_WIDTH; i++) {
                    vga_putc_at(screen_x + i, screen_y + CELL_HEIGHT, '-');
                }
            }
            
            // Draw intersections
            if (x < BOARD_SIZE - 1 && y < BOARD_SIZE - 1) {
                vga_putc_at(screen_x + CELL_WIDTH, screen_y + CELL_HEIGHT, '+');
            }
        }
    }
}

// Draw the cursor
void draw_cursor() {
    if (game_over) return;
    
    int screen_x = BOARD_START_X + cursor_x * CELL_WIDTH;
    int screen_y = BOARD_START_Y + cursor_y * CELL_HEIGHT;
    
    // Draw cursor around the current cell
    for (int i = 0; i < CELL_WIDTH; i++) {
        vga_putc_at(screen_x + i, screen_y, '*');
        vga_putc_at(screen_x + i, screen_y + CELL_HEIGHT - 1, '*');
    }
    
    for (int i = 1; i < CELL_HEIGHT - 1; i++) {
        vga_putc_at(screen_x, screen_y + i, '*');
        vga_putc_at(screen_x + CELL_WIDTH - 1, screen_y + i, '*');
    }
}

// Draw game status
void draw_status() {
    char status[50] = {0};
    
    if (game_over) {
        if (winner == 0) {
            kstrcpy(status, "X wins! Press 'r' to restart, 'q' to quit");
        } else if (winner == 1) {
            kstrcpy(status, "O wins! Press 'r' to restart, 'q' to quit");
        } else if (winner == 2) {
            kstrcpy(status, "Draw! Press 'r' to restart, 'q' to quit");
        }
    } else {
        kstrcpy(status, "Player: ");
        kstrcat(status, current_player == 0 ? "X" : "O");
        kstrcat(status, " - Arrows: move, Space: mark");
    }
    
    // Center the status message
    int status_x = (VGA_WIDTH - kstrlen(status)) / 2;
    for (int i = 0; status[i]; i++) {
        vga_putc_at(status_x + i, VGA_HEIGHT - 2, status[i]);
    }
}

// Check if there's a winner
int check_winner() {
    // Check rows
    for (int y = 0; y < BOARD_SIZE; y++) {
        if (board[y][0] != ' ' && board[y][0] == board[y][1] && board[y][1] == board[y][2]) {
            return board[y][0] == 'X' ? 0 : 1;
        }
    }
    
    // Check columns
    for (int x = 0; x < BOARD_SIZE; x++) {
        if (board[0][x] != ' ' && board[0][x] == board[1][x] && board[1][x] == board[2][x]) {
            return board[0][x] == 'X' ? 0 : 1;
        }
    }
    
    // Check diagonals
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return board[0][0] == 'X' ? 0 : 1;
    }
    
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return board[0][2] == 'X' ? 0 : 1;
    }
    
    // Check for draw
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (board[y][x] == ' ') {
                return -1; // Game not over
            }
        }
    }
    
    return 2; // Draw
}

// Make a move
void make_move() {
    if (board[cursor_y][cursor_x] == ' ' && !game_over) {
        board[cursor_y][cursor_x] = current_player == 0 ? 'X' : 'O';
        
        // Check for winner or draw
        winner = check_winner();
        if (winner != -1) {
            game_over = 1;
        } else {
            // Switch player
            current_player = 1 - current_player;
        }
    }
}

// Handle input
void handle_input(unsigned char scancode) {
    if (scancode < 0x80) {
        if (game_over) {
            if (scancode == 0x10) { // 'Q' key
                return;
            } else if (scancode == 0x13) { // 'R' key
                init_game();
            }
        } else {
            if (scancode == 0x48) { // Up arrow
                cursor_y = (cursor_y - 1 + BOARD_SIZE) % BOARD_SIZE;
            } else if (scancode == 0x50) { // Down arrow
                cursor_y = (cursor_y + 1) % BOARD_SIZE;
            } else if (scancode == 0x4B) { // Left arrow
                cursor_x = (cursor_x - 1 + BOARD_SIZE) % BOARD_SIZE;
            } else if (scancode == 0x4D) { // Right arrow
                cursor_x = (cursor_x + 1) % BOARD_SIZE;
            } else if (scancode == 0x39) { // Space
                make_move();
            }
        }
    }
}


void clear_game_area() {
    // Clear the area where the board and cursor are drawn
    for (int y = BOARD_START_Y; y < BOARD_START_Y + BOARD_SIZE * CELL_HEIGHT + 1; y++) {
        for (int x = BOARD_START_X; x < BOARD_START_X + BOARD_SIZE * CELL_WIDTH + 1; x++) {
            vga_putc_at(x, y, ' ');
        }
    }
    // Also clear status line
    for (int x = 0; x < VGA_WIDTH; x++) {
        vga_putc_at(x, VGA_HEIGHT - 2, ' ');
    }
}

// Main game loop
// Main game loop
void tictactoe_game() {
    init_game();
    
    // Clear screen
    vga_clear();
    
    // Draw title
    char *title = "TIC TAC TOE";
    int title_x = (VGA_WIDTH - kstrlen(title)) / 2;
    for (int i = 0; title[i]; i++) {
        vga_putc_at(title_x + i, 1, title[i]);
    }
    
    // Draw instructions
    char *instructions = "Arrow keys: Move cursor, Space: Place mark";
    int inst_x = (VGA_WIDTH - kstrlen(instructions)) / 2;
    for (int i = 0; instructions[i]; i++) {
        vga_putc_at(inst_x + i, 3, instructions[i]);
    }
    
    int quit = 0;
    while (!quit) {
        // Check if a key is available
        if ((inb(0x64) & 1) != 0) {
            unsigned char scancode = inb(0x60);

            // Handle the input
            handle_input(scancode);

            // Quit only when game is over and 'Q' is pressed
            if (scancode == 0x10 && game_over) { // 'Q'
                quit = 1;
            }
        }

        // Clear old frame and redraw everything
        clear_game_area();
        draw_board();
        draw_cursor();
        draw_status();

        // Simple delay to avoid flickering
        for (volatile int i = 0; i < 50000; i++);
    }
    
    // Return to shell
    vga_clear();
    vga_puts("Returning to shell...\n");
}