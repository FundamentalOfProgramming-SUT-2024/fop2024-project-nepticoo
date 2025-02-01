#include "head.h"

extern USER user;
extern USER_S user_s;

// MENUS
int draw_menu_border();
int main_menu();
int draw_main_menu_options(int selected);
int sign_in_menu();
int draw_sign_in_menu_options(int selected, char* entered_username);
int check_username_existence(char* entered_username);
int check_password_username(char* entered_username, char* entered_password);
int sign_up_menu();
int draw_sign_up_menu_options(int selected, char* entered_username, char* entered_email, char* generated_password);
int check_valid_email(char* entered_email);
int check_valid_password(char* entered_password);
int generate_random_password(char* generated_password);
int create_user_files(char* entered_username, char* entered_email, char* entered_password);
int get_user_info(char* entered_username);

int draw_menu_border() {
    for(int i = 1; i < LINES-1; i++) {
        mvprintw(i, 0, "|");
        mvprintw(i, COLS-1, "|");
    }
    for(int i = 0; i < COLS; i++) {
        mvprintw(0, i, "-");
        mvprintw(LINES-1, i, "-");
    }
    refresh();
}

int draw_main_menu_options(int selected) {
    char options[4][M] = {{"SIGN-IN"}, {"SIGN-UP"}, {"GUEST-MODE"},{"EXIT"}};
    clear();
    attron(COLOR_PAIR(106));
    draw_menu_border();
    attroff(COLOR_PAIR(106));
    attron(A_BOLD | A_BLINK | COLOR_PAIR(108));
    mvprintw(12, 4, "WELLCOME TO THE DUNGEON!");
    attroff(A_BOLD | A_BLINK | COLOR_PAIR(108));
    attron(COLOR_PAIR(106));
    for(int i = 0; i < 4; i++) {
        if(i == selected) {
            attron(A_BOLD | A_REVERSE);
        }
        mvprintw(15 + (2*i), 6, "%s", options[i]);
        if(i == selected) {
            attroff(A_BOLD | A_REVERSE);
        }
    }
    attroff(COLOR_PAIR(106));
    refresh();
}

int main_menu() {
    int selected = 0;
    while(1) {
        draw_main_menu_options(selected);
        int c = getch();
        if(c == KEY_UP) {
            if(selected == 0)
                selected = 3;
            else
                selected--;
        }
        if(c == KEY_DOWN) {
            if(selected == 3)
                selected = 0;
            else
                selected++;
        }
        if(c == KEY_ENT) {
            if(selected == 0) {
                if(!sign_in_menu()) {
                    return 0;
                }
            }
            if(selected == 1) {
                if(!sign_up_menu()) {
                    return 0;
                }
            }
            if(selected == 2) {
                char tmp[50] = "GUEST";
                get_user_info(tmp);
                return 0;
            }
            if(selected == 3) {
                return 1;
            }
        }
    }
}

int check_username_existence(char* entered_username) {
    char path[2 * M] = "./users/";
    strcat(path, entered_username);
    DIR* user_directory = opendir(path);
    if(user_directory) {
        closedir(user_directory);
        return 1;
    }
    else
        return 0;
}

int check_password_username(char* entered_username, char* entered_password) {
    char path[2 * M] = "./users/";
    strcat(path, entered_username);
    strcat(path, "/password.txt");
    FILE* password_file = fopen(path, "r");
    char password[M];
    fscanf(password_file, "%s", password);
    fclose(password_file);
    if(!strcmp(password, entered_password))
        return 1;
    else
        return 0;
}

int draw_sign_in_menu_options(int selected, char* entered_username) {
    char options[2][M] = {{"Enter your Username:"}, {"Enter your Password:"}};
    clear();
    attron(COLOR_PAIR(106));
    draw_menu_border();
    attroff(COLOR_PAIR(106));
    attron(COLOR_PAIR(108));
    attron(A_BOLD | A_BLINK);
    mvprintw(12, 4, "WELLCOME THE OLD WARRIOR!");
    attroff(A_BOLD | A_BLINK | COLOR_PAIR(108));
    attron(A_DIM);
    mvprintw(13, 4, "(to return a step type \"EXIT\" and press ENTER)");
    attroff(A_DIM);
    attroff(COLOR_PAIR(108));
    attron(COLOR_PAIR(106));
    for(int i = 0; i < 2; i++) {
        if(i == selected) {
            attron(A_BOLD | A_REVERSE);
        }
        mvprintw(15 + (3*i), 6, "%s", options[i]);
        if(i == selected) {
            attroff(A_BOLD | A_REVERSE);
        }
    }
    attroff(COLOR_PAIR(106));
    if(selected == 1) {
        attron(COLOR_PAIR(102));
        mvprintw(15, 27, "%s", entered_username);
        attroff(COLOR_PAIR(102));
    }
    refresh();
}

int sign_in_menu() {
    int selected = 0;
    char entered_username[M], entered_password[M];
    curs_set(true);
    echo();
    while(1) {
        draw_sign_in_menu_options(selected, entered_username);
        if(selected == 0) {
            attron(COLOR_PAIR(102));
            mvscanw(15, 27, "%s", entered_username);
            attroff(COLOR_PAIR(102));
            if(!strcmp(entered_username, "EXIT")) {
                curs_set(false);
                noecho();
                return 1;
            }
            if(check_username_existence(entered_username)) {
                selected++;
                continue;
            }
            else {
                attron(COLOR_PAIR(105));
                mvprintw(16, 6, "NO PLAYER WITH THIS USERNAME EXISTS!!!");
                attroff(COLOR_PAIR(105));
                refresh();
                napms(2500);
                continue;
            }
        }
        if(selected == 1) {
            attron(COLOR_PAIR(102));
            mvscanw(18, 27, "%s", entered_password);
            attroff(COLOR_PAIR(102));
            if(!strcmp(entered_password, "EXIT")) {
                selected--;
                continue;
            }
            if(check_password_username(entered_username, entered_password)) {
                attron(COLOR_PAIR(103));
                mvprintw(19, 6, "YOU HAVE SUCCESSFULLY SIGNED IN!!!");
                attroff(COLOR_PAIR(103));
                refresh();
                get_user_info(entered_username);
                curs_set(false);
                noecho();
                napms(2500);
                return 0;
            }
            else {
                attron(COLOR_PAIR(105));
                mvprintw(19, 6, "USERNAME AND PASSWORD DOESN'T MATCH!!!");
                attroff(COLOR_PAIR(105));
                refresh();
                napms(2500);
                continue;
            }
        }
    }
}

int check_valid_email(char* entered_email) {
    char *at = strchr(entered_email, '@');
    char *dot = strrchr(entered_email, '.');
    if (at == NULL || dot == NULL || at > dot || dot == at + 1 || at == entered_email || dot == entered_email + strlen(entered_email) - 1) {
        return 0;
    }
    for (char *ptr = entered_email; ptr < at; ptr++) {
        if (!((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= 'A' && *ptr <= 'Z') ||
                (*ptr >= '0' && *ptr <= '9') || *ptr == '.' || *ptr == '_' || *ptr == '%'
                || *ptr == '+' || *ptr == '-')) {
            return 0;
        }
    }
    for (char *ptr = at + 1; ptr < dot; ptr++) {
        if (!(((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= 'A' && *ptr <= 'Z') ||
                (*ptr >= '0' && *ptr <= '9') || *ptr == '-'))) {
            return 0;
        }
    }
    if (strlen(dot + 1) < 2) {
        return 0;
    }
    return 1;
}

int check_valid_password(char* entered_password) {
    if(strlen(entered_password) < 7)
        return 0;
    int check_upper = 0, check_lower = 0, check_digit = 0;
    for(int i = 0; i < strlen(entered_password); i++) {
        if(entered_password[i] >= 'A' && entered_password[i] <= 'Z')
            check_upper = 1;
        if(entered_password[i] >= 'a' && entered_password[i] <= 'z')
            check_lower = 1;
        if(entered_password[i] >= '0' && entered_password[i] <= '9')
            check_digit = 1;
    }
    if(!check_upper || !check_lower || !check_digit)
        return 0;
    return 1;
}

int generate_random_password(char* generated_password) {
    char upper_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char lower_chars[] = "abcdefghijklmnopqrstuvwxyz";
    char digits[] = "0123456789";
    int lenght = 8 + (rand() % 4);
    int upper_index = rand() % lenght;
    int lower_index = rand() % lenght;
    int digit_index = rand() % lenght;
    generated_password[upper_index] = upper_chars[rand() % strlen(upper_chars)];
    generated_password[lower_index] = lower_chars[rand() % strlen(lower_chars)];
    generated_password[digit_index] = digits[rand() % strlen(digits)];
    char all_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for(int i = 0; i < lenght; i++) {
        if(i != upper_index && i != lower_index && i != digit_index)
            generated_password[i] = all_chars[rand() % strlen(all_chars)];
    }
    generated_password[lenght] = '\0';
}

int draw_sign_up_menu_options(int selected, char* entered_username, char* entered_email, char* generated_password) {
    char options[3][M] = {{"Enter your Username:"}, {"Enter your Email   :"}, {"Enter your Password:"}};
    clear();
    attron(COLOR_PAIR(106));
    draw_menu_border();
    attroff(COLOR_PAIR(106));
    attron(COLOR_PAIR(108));
    attron(A_BOLD | A_BLINK);
    mvprintw(12, 4, "WELLCOME THE NEW WARRIOR!");
    attroff(A_BOLD | A_BLINK | COLOR_PAIR(108));
    attron(A_DIM);
    mvprintw(13, 4, "(to return a step type \"EXIT\" and press ENTER)");
    attroff(A_DIM);
    attroff(COLOR_PAIR(108));
    attron(COLOR_PAIR(106));
    for(int i = 0; i < 3; i++) {
        if(i == selected) {
            attron(A_BOLD | A_REVERSE);
        }
        mvprintw(15 + (3*i), 6, "%s", options[i]);
        if(i == selected) {
            attroff(A_BOLD | A_REVERSE);
        }
    }
    attron(A_DIM);
    mvprintw(22, 6, "(to generate a random password \"RANDOM\" and press ENTER)");
    attroff(A_DIM);
    attroff(COLOR_PAIR(106));
    attron(COLOR_PAIR(102));
    if(selected > 0) {
        mvprintw(15, 27, "%s", entered_username);
    }
    if(selected > 1) {
        mvprintw(18, 27, "%s", entered_email);
    }
    if(selected == 2 && strlen(generated_password) > 0) {
        attron(A_DIM | COLOR_PAIR(102));
        mvprintw(23, 6, "your random password is: %s", generated_password);
        attroff(A_DIM | COLOR_PAIR(102));
    }
    attroff(COLOR_PAIR(102));
    refresh();
}

int sign_up_menu() {
    int selected = 0;
    char entered_username[M], entered_email[M], entered_password[M], generated_password[M];
    generated_password[0] = '\0';
    curs_set(true);
    echo();
    while(1) {
        draw_sign_up_menu_options(selected, entered_username, entered_email, generated_password);
        if(selected == 0) {
            attron(COLOR_PAIR(102));
            mvscanw(15, 27, "%s", entered_username);
            attroff(COLOR_PAIR(102));
            if(!strcmp(entered_username, "EXIT")) {
                curs_set(false);
                noecho();
                return 1;
            }
            if(!check_username_existence(entered_username)) {
                selected++;
                continue;
            }
            else {
                attron(COLOR_PAIR(105));
                mvprintw(16, 6, "THIS USERNAME ALREADY EXISTS!!!");
                attroff(COLOR_PAIR(105));
                refresh();
                napms(2500);
                continue;
            }
        }
        if(selected == 1) {
            attron(COLOR_PAIR(102));
            mvscanw(18, 27, "%s", entered_email);
            attroff(COLOR_PAIR(102));
            if(!strcmp(entered_email, "EXIT")) {
                selected--;
                continue;
            }
            if(check_valid_email(entered_email)) {
                selected++;
                continue;
            }
            else {
                attron(COLOR_PAIR(105));
                mvprintw(19, 6, "YOUR EMAIL IS INVALID!!!");
                attroff(COLOR_PAIR(105));
                refresh();
                napms(2500);
                continue;
            }
        }
        if(selected == 2) {
            attron(COLOR_PAIR(102));
            mvscanw(21, 27, "%s", entered_password);
            attroff(COLOR_PAIR(102));
            if(!strcmp(entered_password, "EXIT")) {
                selected--;
                generated_password[0] = '\0';
                continue;
            }
            if(!strcmp(entered_password, "RANDOM")) {
                generate_random_password(generated_password);
                continue;
            }
            if(check_valid_password(entered_password)) {
                int is_generated = 0;
                if(strlen(generated_password) > 0)
                    is_generated = 1;
                attron(COLOR_PAIR(103));
                mvprintw(23 + is_generated, 6, "YOU HAVE SUCCESSFULLY SIGNED UP!!!");
                attroff(COLOR_PAIR(103));
                refresh();
                create_user_files(entered_username, entered_email, entered_password);
                get_user_info(entered_username);
                curs_set(false);
                noecho();
                napms(2500);
                return 0;
            }
            else {
                int is_generated = 0;
                if(strlen(generated_password) > 0)
                    is_generated = 1;
                attron(COLOR_PAIR(105));
                if(strlen(entered_password) < 7) {
                    mvprintw(23 + is_generated, 6, "PASSWORD MUST BE MORE THAN 7 CHARACTERS!!!");
                }
                else {
                    mvprintw(23 + is_generated, 6, "PASSWORD MUST CONTAIN AT LEAST ONE LOWER-CASE, UPPER-CASE AND DIGIT!!!");
                }
                attroff(COLOR_PAIR(105));
                refresh();
                napms(2500);
                continue;
            }
        }
    }
}

int create_user_files(char* entered_username, char* entered_email, char* entered_password) {
    strcpy(user.username, entered_username);
    strcpy(user.email, entered_email);
    strcpy(user.password, entered_password);
    user.is_guest = 0;
    user.has_load = 0;
    user.difficulty = 0;
    user.all_saved_golds = 0;
    user.points = 0;
    user.first_game_time = time(NULL);
    user.theme_code = 0;
    user.finished_games = 0;
    user.music_stat = 0;

    char path[2 * M] = "./users/";
    strcat(path, entered_username);
    mkdir(path, 0700);
    strcat(path, "/user.dat");
    FILE* user_file = fopen(path, "wb");
    fwrite(&user, sizeof(USER), 1, user_file);
    fclose(user_file);
    strcpy(path, "./users/");
    strcat(path, entered_username);
    closedir(opendir(path));

    strcpy(user_s.username, entered_username);
    user_s.all_saved_golds = 0;
    user_s.points = 0;
    user_s.finished_games = 0;
    user_s.first_game_time = time(NULL);
    strcat(path, "/user_s.dat");
    FILE* user_s_file = fopen(path, "wb");
    fwrite(&user_s, sizeof(USER_S), 1, user_s_file);
    fclose(user_s_file);
}

int get_user_info(char* entered_username) {
    if(!strcmp(entered_username, "GUEST")) {
        strcpy(user.username, "GUEST");
        strcpy(user.email, "NONE");
        strcpy(user.password, "NONE");
        user.is_guest = 1;
        user.has_load = 0;
        user.difficulty = 0;
        user.all_saved_golds = 0;
        user.points = 0;
        user.first_game_time = time(NULL);
        user.theme_code = 0;
        user.finished_games = 0;
        user.music_stat = 0;
        return 0;
    }
    char path[2 * M] = "./users/";
    strcat(path, entered_username);
    strcat(path, "/user.dat");
    FILE* user_file = fopen(path, "rb");
    fread(&user, sizeof(USER), 1, user_file);
    fclose(user_file);
    return 0;
}