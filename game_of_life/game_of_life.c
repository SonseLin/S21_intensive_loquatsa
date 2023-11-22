#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

// field frame
#define HEIGHT 25
#define WIDTH 80

// speed adjust
#define BASE_SPEED 3
#define MAX_SPEED 6
#define MIN_SPEED 1

// cell state
#define DEAD 0
#define ALIVE 1

// keys
#define FASTER 's'
#define SLOWER 'f'
#define QUIT 'q'

// colors
#define RED 1
#define WHITE 2
#define CYAN 3
#define YELLOW 4
#define BLUE 5
#define BLACK 6
#define GREEN 100

/*
    ABOUT INTERACTIVE PART
*/
// inits interactive mode
void init_curses();
// colors
void shiza_mode();
// ends interactive mode
void the_magic_ends_here(int in_process);

/*
    COLORS AND OUTPUT OVERALL
*/
// shizophrenik color output
void print_empty_speed_bar();
void print_speed_bar();
void print_colorful_cell(int **gen, int r, int c);
// frame here
void output_speed(int speed);
void print_Y_border();
void print_X_border(int i, int column);
void draw_screen(int **present_gen, int speed, int gen);

/*
    ABOUT WORKING WITH MATRIXES
*/
// allocation size for matrix
int **alloc_matrix(int row, int column);
// fill matrix from input
int scan_input_generation(int **matrix, int row, int column);
// print matrix
void output_generation(int **gen, int row, int column);
// copy matrix to another matrix
void copy_generation(int **from, int row, int column, int **to);
// compare 2 matrixes
int the_same_matrix(int **prev_gen, int row, int column, int **next_gen);

/*
    GAME LOGIC ITSELF
*/
// GAME OF LIFE.c
void game_of_life(int **present_gen, int **past_gen, int **next_gen);
// basic setup
void setup_requirements(int *gen, int *speed, int *in_process, int **present_gen, int **past_gen,
                        int **next_gen);
// checks for game end
int game_run(int **past_gen, int **next_gen, int in_process);
// calculate generation
void generation_process(int *in_process, int *speed, int **present_gen, int **past_gen, int **next_gen,
                        int *gen);
// count cells around
int count_neighbours(int **matrix, int y_pos, int x_pos);
// checks will cell alive or not
int will_alive_cell(int **matrix, int y_pos, int x_pos);
// calculate next field
void fill_generation(int **matrix, int row, int column, int **next_gen);
// life function
void evolution(int **present_gen, int **past_gen, int **next_gen, int *gen);

/*
    USER INPUT
*/
// get user input for speed adjust
int get_key(int *speed);

int main() {
    int **present_gen = alloc_matrix(HEIGHT, WIDTH);
    int **past_gen = alloc_matrix(HEIGHT, WIDTH);
    int **next_gen = alloc_matrix(HEIGHT, WIDTH);
    game_of_life(present_gen, past_gen, next_gen);
    if (next_gen != NULL) free(next_gen);
    if (present_gen != NULL) free(present_gen);
    if (past_gen != NULL) free(past_gen);
    return 0;
}

void init_curses() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, 1);
}

void shiza_mode() {
    start_color();
    init_pair(RED, COLOR_RED, COLOR_RED);
    init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);
    init_pair(CYAN, COLOR_CYAN, COLOR_CYAN);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLUE);
    init_pair(BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_GREEN);
}

void the_magic_ends_here(int in_process) {
    refresh();
    nodelay(stdscr, !in_process);
    getch();
    endwin();
}

void print_empty_speed_bar() {
    attron(COLOR_PAIR(WHITE));
    printw("|");
    attroff(COLOR_PAIR(WHITE));
}

void print_speed_bar() {
    attron(COLOR_PAIR(GREEN));
    printw("|");
    attroff(COLOR_PAIR(GREEN));
}

void print_colorful_cell(int **gen, int r, int c) {
    double random = r * c / 3 + (c * r) % (c + 1);
    double color = (int)random % 5 + 1;
    if (gen[r][c] == 1) attron(COLOR_PAIR((int)color));
    if (gen[r][c] == 0) attron(COLOR_PAIR(BLACK));
    printw("%d", gen[r][c]);
    attroff(COLOR_PAIR((int)color));
    attroff(COLOR_PAIR(BLACK));
}

void output_speed(int speed) {
    printw("Speed: ");
    for (int i = MAX_SPEED; i > speed - 1; i--) print_speed_bar();
    for (int i = speed; i > 1; i--) print_empty_speed_bar();
    printw("\n");
}

void print_Y_border() {
    attron(COLOR_PAIR(YELLOW));
    printw("%d", 0);
    attroff(COLOR_PAIR(YELLOW));
}

void print_X_border(int i, int column) {
    attron(COLOR_PAIR(YELLOW));
    i + 1 < column + 2 ? printw("%d", 0) : printw("%d\n", 0);
    attroff(COLOR_PAIR(YELLOW));
}

void draw_screen(int **present_gen, int speed, int gen) {
    clear();
    printw("Generation: %d\n", gen);
    output_speed(speed);
    output_generation(present_gen, HEIGHT, WIDTH);
    printw("Press 'q' to quit the game\n      's' to slow the speed\n      'f' to raise the speed");
    refresh();
    napms(speed * 50);
}

int **alloc_matrix(int row, int column) {
    int **data = malloc(row * column * sizeof(int) + row * sizeof(int *));
    int *ptr = (int *)(data + row);
    for (int i = 0; i < row; i++) data[i] = ptr + column * i;
    return data;
}

int scan_input_generation(int **matrix, int row, int column) {
    int res = 1;
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < column; c++) {
            int num;
            if (scanf("%d", &num) == 1 && (num == 1 || num == 0))
                matrix[r][c] = num;
            else {
                res = 0;
                break;
            }
        }
    }
    return res;
}

void output_generation(int **gen, int row, int column) {
    for (int i = 0; i < column + 2; i++) print_X_border(i, column);
    for (int r = 0; r < row; r++) {
        print_Y_border();
        for (int c = 0; c < column; c++) print_colorful_cell(gen, r, c);
        print_Y_border();
        if (r + 1 < row) printw("\n");
    }
    printw("\n");
    for (int i = 0; i < column + 2; i++) print_X_border(i, column);
}

void copy_generation(int **from, int row, int column, int **to) {
    for (int r = 0; r < row; r++)
        for (int c = 0; c < column; c++) to[r][c] = from[r][c];
}

int the_same_matrix(int **prev_gen, int row, int column, int **next_gen) {
    int res = 1;
    for (int r = 0; r < row; r++)
        for (int c = 0; c < column; c++)
            if (prev_gen[r][c] != next_gen[r][c]) {
                res = 0;
                break;
            }
    return res;
}

void game_of_life(int **present_gen, int **past_gen, int **next_gen) {
    if (present_gen != NULL && past_gen != NULL && next_gen != NULL) {
        if (scan_input_generation(present_gen, HEIGHT, WIDTH)) {
            int gen, speed, in_process;
            setup_requirements(&gen, &speed, &in_process, present_gen, past_gen, next_gen);
            while (game_run(past_gen, next_gen, in_process))
                generation_process(&in_process, &speed, present_gen, past_gen, next_gen, &gen);
            the_magic_ends_here(in_process);
        } else {
            printf("Wrong text file structure");
        }
    }
}

void setup_requirements(int *gen, int *speed, int *in_process, int **present_gen, int **past_gen,
                        int **next_gen) {
    *gen = 0, *speed = BASE_SPEED, *in_process = 1;
    init_curses();
    shiza_mode();
    (void)freopen("/dev/tty", "rw", stdin);
    fill_generation(present_gen, HEIGHT, WIDTH, next_gen);
    copy_generation(present_gen, HEIGHT, WIDTH, past_gen);
}

int game_run(int **past_gen, int **next_gen, int in_process) {
    return (!the_same_matrix(past_gen, HEIGHT, WIDTH, next_gen) && in_process);
}

void generation_process(int *in_process, int *speed, int **present_gen, int **past_gen, int **next_gen,
                        int *gen) {
    *in_process = get_key(speed);
    draw_screen(present_gen, *speed, *gen);
    evolution(present_gen, past_gen, next_gen, gen);
}

int count_neighbours(int **matrix, int y_pos, int x_pos) {
    int neighbours = 0;
    for (int r = y_pos - 1; r <= y_pos + 1; r++)
        for (int c = x_pos - 1; c <= x_pos + 1; c++)
            if (matrix[(r + HEIGHT) % HEIGHT][(c + WIDTH) % WIDTH] == 1) neighbours++;
    if (matrix[y_pos][x_pos] == ALIVE) neighbours--;
    return neighbours;
}

int will_alive_cell(int **matrix, int y_pos, int x_pos) {
    int will = DEAD;
    int neighbours = count_neighbours(matrix, y_pos, x_pos);
    if (matrix[y_pos][x_pos] == ALIVE && (neighbours == 2 || neighbours == 3))
        will = ALIVE;
    else if (matrix[y_pos][x_pos] == DEAD && neighbours == 3)
        will = ALIVE;
    return will;
}

void fill_generation(int **matrix, int row, int column, int **next_gen) {
    for (int r = 0; r < row; r++)
        for (int c = 0; c < column; c++) next_gen[r][c] = will_alive_cell(matrix, r, c);
}

void evolution(int **present_gen, int **past_gen, int **next_gen, int *gen) {
    *gen += 1;
    copy_generation(present_gen, HEIGHT, WIDTH, past_gen);
    fill_generation(present_gen, HEIGHT, WIDTH, next_gen);
    copy_generation(next_gen, HEIGHT, WIDTH, present_gen);
    fill_generation(present_gen, HEIGHT, WIDTH, next_gen);
}

int get_key(int *speed) {
    int res = 1;
    char key = getch();
    if ((key == FASTER) && *speed < MAX_SPEED)
        *speed += 1;
    else if ((key == SLOWER) && *speed > MIN_SPEED)
        *speed -= 1;
    else if (key == QUIT)
        res = 0;
    return res;
}
