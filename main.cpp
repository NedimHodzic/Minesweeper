#include <iostream>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>

using namespace std;

char game_board[9][9];
char printed_board[9][9];

void nearby_mines(int x, int y) {
    int count = 0;

    if(y - 1 >= 0 && x - 1 >= 0) {
        if(game_board[y - 1][x - 1] == 'o') {
            count++;
        }
    }
    if(y - 1 >= 0) {
        if(game_board[y - 1][x] == 'o') {
            count++;
        }
    }
    if(y - 1 >= 0 && x + 1 < 9) {
        if(game_board[y - 1][x + 1] == 'o') {
            count++;
        }
    }
    if(x - 1 >= 0) {
        if(game_board[y][x - 1] == 'o') {
            count++;
        }
    }
    if(x + 1 < 9) {
        if(game_board[y][x + 1] == 'o') {
            count++;
        }
    }
    if(y + 1 < 9 && x - 1 >= 0) {
        if(game_board[y + 1][x - 1] == 'o') {
            count++;
        }
    }
    if(y + 1 < 9) {
        if(game_board[y + 1][x] == 'o') {
            count++;
        }
    }
    if(y + 1 < 9 && x + 1 < 9) {
        if(game_board[y + 1][x + 1] == 'o') {
            count++;
        }
    }

    count += 48;
    printed_board[y][x] = (char)count;
}

void init_game() {
    int x,y,i;
    i = 0;

    for(y = 0; y < 9; y++) {
        for(x = 0; x < 9; x++) {
            game_board[y][x] = '-';
            printed_board[y][x] = '-';
        }
    }

    while(i < 10) {
        x = rand() % 9;
        y = rand() % 9;

        if(game_board[y][x] != 'o') {
            game_board[y][x] = 'o';
            i++;
        }
    }
}

void print_board() {
    int x,y;

    start_color();
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(4, COLOR_RED, COLOR_GREEN);
    init_pair(5, COLOR_GREEN, COLOR_GREEN);
    init_pair(6, COLOR_BLACK, COLOR_GREEN);

    attrset(COLOR_PAIR(2));
    mvprintw(10, 0, "Welcome to minesweeper. There are 10 bombs.");
    mvprintw(11, 0, "Use WASD to move the cursor, x to select a space, and b to mark a space as a bomb.");
    attroff(COLOR_PAIR(2));

    for(y = 0; y < 9; y++) {
        for(x = 0; x < 9; x++) {
            if(printed_board[y][x] == '-') {
                attrset(COLOR_PAIR(3));
                mvprintw(y, x, "%c", printed_board[y][x]);
                attroff(COLOR_PAIR(3));
            }
            else if(printed_board[y][x] == 'B') {
                attrset(COLOR_PAIR(4));
                mvprintw(y, x, "%c", printed_board[y][x]);
                attroff(COLOR_PAIR(4));
            }
            else if(printed_board[y][x] == '0') {
                attrset(COLOR_PAIR(5));
                mvprintw(y, x, "%c", printed_board[y][x]);
                attroff(COLOR_PAIR(5));
            }
            else {
                attrset(COLOR_PAIR(6));
                mvprintw(y, x, "%c", printed_board[y][x]);
                attroff(COLOR_PAIR(6));
            }
        }
    }
}

void init_terminal() {
    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
}

bool is_filled() {
    int x, y;
    bool is_filled = true;

    for(y = 0; y < 9; y++) {
        for(x = 0; x < 9; x++) {
            if(printed_board[y][x] == '-') {
                is_filled = false;
            }
        }
    }

    return is_filled;
}

int main(int argc, char *argv[]) {
    int player_x, player_y;
    player_x = player_y = 0;
    char player_action;

    srand(time(NULL));

    init_terminal();
    init_game();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_WHITE);

    while(true) {
        print_board();

        attrset(COLOR_PAIR(1));
        mvprintw(player_y, player_x, "P");
        attroff(COLOR_PAIR(1));

        player_action = getch();

        if(player_action == 'q' || player_action == 'Q') {
            clear();
            mvprintw(0, 0, "Game quit!");
            break;
        }
        else if(player_action == 'x') {
            if(game_board[player_y][player_x] == 'o') {
                clear();
                mvprintw(0, 0, "You landed on a mine, you lose!");
                break;
            }
            else {
                nearby_mines(player_x, player_y);
            }
        }
        else if(player_action == 'b') {
            printed_board[player_y][player_x] = 'B';
        }
        else if(player_action == 'w') {
            if(player_y - 1 >= 0) {
                player_y--;
            }
        }
        else if(player_action == 'a') {
            if(player_x - 1 >= 0) {
                player_x--;
            }
        }
        else if(player_action == 's') {
            if(player_y + 1 < 9) {
                player_y++;
            }
        }
        else if(player_action == 'd') {
            if(player_x + 1 < 9) {
                player_x++;
            }
        }
        else {
            mvprintw(13, 0, "Not a valid action.");
            getch();
            mvprintw(13, 0, "                    ");
        }

        if(is_filled()) {
            clear();
            mvprintw(0, 0, "You won!");
            break;
        }
    }

    getch();
    endwin();

    return 0;
}
