#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define FIELD_WIDTH 80   // ширина поля
#define FIELD_HEIGTH 25  // высота поля
#define END_SCORE 21     // условия завершения игры

#define BALL_POS_DEF_X FIELD_WIDTH / 2   // позиция мяча по ширине
#define BALL_POS_DEF_Y FIELD_HEIGTH / 2  // позиция мяча по высоте
#define BALL_DIFECTION 00                // направление мяча
#define BALL_BOUNCE 1                    // погрешность мяча

#define RACKET_DENSITY FIELD_WIDTH / 20  // расстояние от ракетки до границы
#define RACKET_POS_1_DEF_X RACKET_DENSITY  // расстояние от ракетки до границы первого игрока
#define RACKET_POS_2_DEF_X FIELD_WIDTH - RACKET_DENSITY  // расстояние от ракетки до границы второго игрока
#define RACKET_EDGE (RACKET_SIZE + 1) / 2  // площадь покрытия ракетки для удара
#define RACKET_SIZE 3                      // размер ракетки

// установка хеллоуинских проклятий(интерактив)
void curse_mode_init();
void update_curse_view();
void end_game(char winner);

// функция отрисовывает поле и мячи
void draw_field(int ball_x, int ball_y, int p1_x, int p2_x, int p1_score, int p2_score);
// показывает окошко с победителем
void show_end_game_screen(char winner);
// вовзвращет нажатую пользователем клавишу
char get_key();

// движение мяча
int movement_ball(int ball_x, int ball_y, int difection);
// движение первого игрока
int movement_p1(int p1_y, char key);
// движение второго игрока
int movement_p2(int p2_y, char key);
// коллизия о ракетку
int racket_punch(int p1_y, int p2_y, int ball_pos_x, int ball_pos_y);
// изменение позиции мяча
int reflect_defection(int ball_defection);
// AI player 2 (bonus option)
int AI_movement_p2(int p2_y, int ball_pos_x, int ball_pos_y);
// ping pong itself
char gameplay(int p1_score, int p2_score, int ball_pos_x, int ball_pos_y, int ball_defection, int p1_y,
              int p2_y, char mode);

// menu
void start_game();

int main() {
    // первоначальный сетап
    int ball_pos_x = BALL_POS_DEF_X;
    int ball_pos_y = BALL_POS_DEF_Y;
    int p1_score = 0;
    int p2_score = 0;
    int p1_y = FIELD_HEIGTH / 2;
    int p2_y = FIELD_HEIGTH / 2;
    int ball_defection = BALL_DIFECTION;
    start_game();
    char mode = (char)getchar();
    curse_mode_init();
    end_game(gameplay(p1_score, p2_score, ball_pos_x, ball_pos_y, ball_defection, p1_y, p2_y, mode));
    return 0;
}

void curse_mode_init() {
    initscr();
    cbreak();
    noecho();
    timeout(0);
}
void update_curse_view() {
    refresh();
    usleep(100000);
    clear();
}
void end_game(char winner) {
    endwin();
    show_end_game_screen(winner);
}

void draw_field(int ball_x, int ball_y, int p1_y, int p2_y, int p1_score, int p2_score) {
    for (int y = 0; y < FIELD_HEIGTH + 1; y++) {
        for (int x = 0; x < FIELD_WIDTH + 1; x++) {
            if (y == 0 || y == FIELD_HEIGTH) {
                printw("#");
            } else if (x == 0 || x == FIELD_WIDTH) {
                printw("#");
            } else if (y == ball_y && x == ball_x) {
                printw("*");
            } else if (x == FIELD_WIDTH / 2) {
                printw("|");
            } else if ((x == RACKET_POS_1_DEF_X &&
                        (y >= p1_y - RACKET_SIZE / 2 && y <= p1_y + RACKET_SIZE / 2)) ||
                       (x == RACKET_POS_2_DEF_X &&
                        (y >= p2_y - RACKET_SIZE / 2 && y <= p2_y + RACKET_SIZE / 2))) {
                printw("|");
            } else {
                printw(" ");
            }
        }
        printw("\n");
    }
    printw("score_1: %d\nscore_2: %d\n", p1_score, p2_score);
}

void show_end_game_screen(char winner) {
    // заменить
    printf("\n\nPlayer %c wins\n\n", winner);
}

void clear_terminal() { printw("\e[1;1H\e[2J"); }

char get_key() { return (char)getchar(); }

int movement_ball(int ball_x, int ball_y, int difection) {
    if (difection == 10) {
        if (ball_y - BALL_BOUNCE <= 0 && ball_x + BALL_BOUNCE < FIELD_WIDTH) difection = 11;
    } else if (difection == 11) {
        if (ball_y + BALL_BOUNCE >= FIELD_HEIGTH && ball_x + BALL_BOUNCE < FIELD_WIDTH) difection = 10;
    } else if (difection == 00) {
        if (ball_y - BALL_BOUNCE <= 0 && ball_x - BALL_BOUNCE > 0) difection = 01;
    } else if (difection == 01) {
        if (ball_y + BALL_BOUNCE >= FIELD_HEIGTH && ball_x - BALL_BOUNCE > 0) difection = 00;
    }
    return difection;
}

int movement_p1(int p1_y, char key) {
    if ((key == 'a' || key == 'A') && p1_y + RACKET_EDGE > 2 * RACKET_EDGE) {
        p1_y--;
    } else if ((key == 'z' || key == 'Z') && p1_y - RACKET_EDGE < FIELD_HEIGTH - 2 * RACKET_EDGE) {
        p1_y++;
    }
    return p1_y;
}

int movement_p2(int p2_y, char key) {
    if ((key == 'k' || key == 'K') && p2_y + RACKET_EDGE > 2 * RACKET_EDGE) {
        p2_y--;
    } else if ((key == 'm' || key == 'M') && p2_y - RACKET_EDGE < FIELD_HEIGTH - 2 * RACKET_EDGE) {
        p2_y++;
    }
    return p2_y;
}

int racket_punch(int p1_y, int p2_y, int ball_pos_x, int ball_pos_y) {
    int is_punched = 0;
    if (ball_pos_x == RACKET_POS_1_DEF_X + 1 &&
        (ball_pos_y >= p1_y - RACKET_EDGE && ball_pos_y <= p1_y + RACKET_EDGE))
        is_punched++;
    else if (ball_pos_x == RACKET_POS_2_DEF_X - 1 &&
             (ball_pos_y >= p2_y - RACKET_EDGE && ball_pos_y <= p2_y + RACKET_EDGE))
        is_punched++;
    return is_punched;
}

int reflect_defection(int ball_defection) {
    if (ball_defection >= 10)
        ball_defection -= 10;
    else
        ball_defection += 10;
    return ball_defection;
}

int AI_movement_p2(int p2_y, int ball_pos_x, int ball_pos_y) {
    if (ball_pos_x > FIELD_WIDTH / 2) {
        if (ball_pos_y >= p2_y) {
            if (p2_y - RACKET_EDGE < FIELD_HEIGTH - 2 * RACKET_EDGE) p2_y++;
        } else {
            if (p2_y + RACKET_EDGE > 2 * RACKET_EDGE) p2_y--;
        }
    }
    return p2_y;
}

char gameplay(int p1_score, int p2_score, int ball_pos_x, int ball_pos_y, int ball_defection, int p1_y,
              int p2_y, char mode) {
    while (p1_score < END_SCORE && p2_score < END_SCORE) {
        char key = getch();
        // изменяем положение ракеток
        p1_y = movement_p1(p1_y, key);
        if (mode == '2') {
            p2_y = AI_movement_p2(p2_y, ball_pos_x, ball_pos_y);
        } else {
            p2_y = movement_p2(p2_y, key);
        }
        // изменяем положение мяча
        if (racket_punch(p1_y, p2_y, ball_pos_x, ball_pos_y)) {
            ball_defection = reflect_defection(ball_defection);
        }
        ball_defection = movement_ball(ball_pos_x, ball_pos_y, ball_defection);
        ball_defection >= 10 ? ball_pos_x++ : ball_pos_x--;
        ball_defection % 10 ? ball_pos_y++ : ball_pos_y--;
        draw_field(ball_pos_x, ball_pos_y, p1_y, p2_y, p1_score, p2_score);
        if (ball_pos_x > FIELD_WIDTH - BALL_BOUNCE || ball_pos_x < BALL_BOUNCE) {
            ball_pos_x > FIELD_WIDTH / 2 ? p1_score++ : p2_score++;
            ball_pos_x = BALL_POS_DEF_X;
            ball_pos_y = BALL_POS_DEF_Y;
            ball_defection = reflect_defection(ball_defection);
        }
        update_curse_view();
    }
    return p1_score > p2_score ? '1' : '2';
}

void start_game() {
    printf("#################################################################################\n");
    printf("#                                                                               #\n");
    printf("#                                                                               #\n");
    printf("#              ########       #####      ###   ###      #####                   #\n");
    printf("#              #########     ### ###     ###   ###     ### ###                  #\n");
    printf("#              ###   ###    ###   ###    ####  ###    ###   ###                 #\n");
    printf("#              #########    ###   ###    ##### ###    ###                       #\n");
    printf("#              ########     ###   ###    #########    ###                       #\n");
    printf("#              ###          ###   ###    ### #####    ###  ####                 #\n");
    printf("#              ###          ###   ###    ###  ####    ###   ###                 #\n");
    printf("#              ###           ### ###     ###   ###     ### ###                  #\n");
    printf("#              ###            #####      ###   ###      #####                   #\n");
    printf("#                                                                               #\n");
    printf("#                                                                               #\n");
    printf("#                   ###   ###    ###      #####         ####                    #\n");
    printf("#                   ###   ###    ###     ### ###       #####                    #\n");
    printf("#                   ###   ###    ###    ###   ###     ######                    #\n");
    printf("#                   ###   ###           ###   ###    ### ###                    #\n");
    printf("#                    ### ###            ###   ###        ###                    #\n");
    printf("#                    ### ###            ###   ###        ###                    #\n");
    printf("#                    ### ###     ###    ###   ###        ###                    #\n");
    printf("#                     #####      ###     ### ###         ###                    #\n");
    printf("#                      ###       ###      #####          ###                    #\n");
    printf("#                                                                               #\n");
    printf("#                                                                               #\n");
    printf("#################################################################################\n");
    printf("Choose game mode:\n1) Classic(Player vs Player)\n2) AI(Player vs AI)\n");
}
