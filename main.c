#include "head.h"

USER user;
USER_S user_s;

int pre_procces();
#include "main_menu.c"
#include "pre_game_menu.c" 
#include "init_game.c" 
#include "play_game.c"
 
int win_game();
int lose_game(); 
int save_game();

int main(){
    pre_procces(); 
    if(LINES < 35 || COLS < 120) {
        endwin();
        printf("Please open a larger terminal window (minimum 35 rows and 120 columns) to play the game\n");
        printf("you can zoom out using ctrl + -\n");
        return 1;
    }
    while(1) {
        if(main_menu()) {
            endwin();
            return 0;
        }
        while(1) {
            int pre_game_menu_stat = pre_game_menu();
            if(pre_game_menu_stat == 0) {
                initialize_new_game();
            }
            else if(pre_game_menu_stat == 1) {
                initialize_load_game();
            }
            else {
                break; 
            }
            int game_stat = play_game();
            update_music(0);
            if(game_stat == 0) {
                win_game();
            }
            else if(game_stat == 1) {
                lose_game();
            }
            else if(game_stat == 2) {
                user.has_load = 1;
                save_game();
            }
        }
    }
    
    getch();
    endwin();
    return 0;
} 
 
int pre_procces() {
    srand(time(NULL));
    setlocale(LC_ALL, "");
    initscr();
    curs_set(false);
    noecho();
    keypad(stdscr, true);
    cbreak();
    DIR* users = opendir("./users");
    if(users) {
        closedir(users);
    }
    else {
        mkdir("./users", 0700);
    }
    if(!has_colors()) {
        endwin();
        printf("Your terminal does not support colors\n");
        return 1;
    }
    start_color();
    init_pair(101, COLOR_BLUE, COLOR_BLACK);
    init_pair(102, COLOR_CYAN, COLOR_BLACK);
    init_pair(103, COLOR_GREEN, COLOR_BLACK);
    init_pair(104, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(105, COLOR_RED, COLOR_BLACK);
    init_pair(106, COLOR_YELLOW, COLOR_BLACK);
    init_pair(107, COLOR_WHITE, COLOR_BLACK); 

    // 8 = orange
    init_color(8, 996, 500, 0);
    init_pair(108, 8, COLOR_BLACK);
    
    // 109 = gold
    init_color(109, 797, 797, 0);
    init_pair(109, 109, COLOR_BLACK);

    // 110 = siver
    init_color(110, 755, 755, 755);
    init_pair(110, 110, COLOR_BLACK);

    // // 111 = gold
    init_color(111, 804, 498, 196);
    init_pair(111, 111, COLOR_BLACK);
}

int win_game() {
    int golds = user.golds;
    int points = (golds + (5 * user.health) + 500) * (user.difficulty + 1);
    const char *ascii_art[] = {
        "__   _____  _   _  __        _____ _   _   _   _   _ ",
        "\\ \\ / / _ \\| | | | \\ \\      / /_ _| \\ | | | | | | | |",
        " \\ V / | | | | | |  \\ \\ /\\ / / | ||  \\| | | | | | | |",
        "  | || |_| | |_| |   \\ V  V /  | || |\\  | |_| |_| |_|",
        "  |_| \\___/ \\___/     \\_/\\_/  |___|_| \\_| (_) (_) (_)"
    };
    clear();
    attron(COLOR_PAIR(106));
    draw_menu_border();
    attroff(COLOR_PAIR(106));
    attron(COLOR_PAIR(108) | A_BLINK);
    for(int i = 0; i < 5; i++) {
        mvprintw(11 + i,  + (COLS - strlen(ascii_art[i])) / 2, "%s", ascii_art[i]);
    }
    attroff(COLOR_PAIR(108) | A_BLINK);
    attron(A_BOLD | A_UNDERLINE | COLOR_PAIR(106));
    mvprintw(19, COLS / 2 - 20, "You have collected %d golds and %d points", golds, points);
    attroff(A_BOLD | A_UNDERLINE | COLOR_PAIR(106));
    attron(A_DIM);
    mvprintw(21, COLS / 2 - 15, "(press any key to continue...)");
    attroff(A_DIM);
    refresh();
    getch();

    user.has_load = 0;
    user.all_saved_golds += golds;
    user.points += points;
    user.finished_games += 1;
    save_game();

    user_s.all_saved_golds += golds;
    user_s.points += points;
    user_s.finished_games += 1;
    if(!user.is_guest) {
        char path[2 * M] = "./users/";
        strcat(path, user.username);
        strcat(path, "/user_s.dat");
        FILE* user_s_file = fopen(path, "wb");
        fwrite(&user_s, sizeof(USER_S), 1, user_s_file);
        fclose(user_s_file);
    }
}

int lose_game() {
    const char *ascii_art[] = {
    "  ____    _    __  __ _____    _____     _______ ____    _   _   _ ",
    " / ___|  / \\  |  \\/  | ____|  / _ \\ \\   / / ____|  _ \\  | | | | | |",
    "| |  _  / _ \\ | |\\/| |  _|   | | | \\ \\ / /|  _| | |_) | | | | | | |",
    "| |_| |/ ___ \\| |  | | |___  | |_| |\\ V / | |___|  _ <  |_| |_| |_|",
    " \\____/_/   \\_\\_|  |_|_____|  \\___/  \\_/  |_____|_| \\_\\ (_) (_) (_)"
    };
    clear();
    attron(COLOR_PAIR(106));
    draw_menu_border();
    attroff(COLOR_PAIR(106));
    attron(COLOR_PAIR(108) | A_BLINK);
    for(int i = 0; i < 5; i++) {
        mvprintw(11 + i,  + (COLS - strlen(ascii_art[i])) / 2, "%s", ascii_art[i]);
    }
    attroff(COLOR_PAIR(108) | A_BLINK);
    attron(A_BOLD | A_UNDERLINE | COLOR_PAIR(106));
    mvprintw(19, COLS / 2 - 12, "Maybe another time buddy");
    attroff(A_BOLD | A_UNDERLINE | COLOR_PAIR(106));
    attron(A_DIM);
    mvprintw(21, COLS / 2 - 15, "(press any key to continue...)");
    attroff(A_DIM);
    refresh();
    getch();

    user.has_load = 0;
    save_game();
}

int save_game() {
    if(user.is_guest)
        return 0;
    char path[2 * M] = "./users/";
    strcat(path, user.username);
    strcat(path, "/user.dat");
    FILE *user_file = fopen(path, "wb");
    fwrite(&user, sizeof(USER), 1, user_file);
    fclose(user_file);
    return 0;
    
} 