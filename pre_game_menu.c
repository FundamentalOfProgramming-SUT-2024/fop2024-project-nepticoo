#include "head.h"

extern USER user;
extern USER_S user_s;

int draw_pre_game_menu_options(int selected);
int pre_game_menu();
int draw_settings_menu_options(int selected);
int settings_menu();
int theme_menu();
int difficulty_menu();
int music_menu();
int profile_menu();
int get_all_users_scoreboard(USER_S* all_users);
int scoreboard_menu();


int draw_pre_game_menu_options(int selected) {
    char options[6][M] = {{"NEW GAME"}, {"LOAD GAME"}, {"SETTINGS"},{"PROFILE"}, {"SCOREBOARD"}, {"LOG-OUT"}};
    clear();
    attron(COLOR_PAIR(106));
    draw_menu_border();
    attroff(COLOR_PAIR(106));
    attron(A_BOLD | A_BLINK | COLOR_PAIR(108));
    mvprintw(12, 4, "WELLCOME TO THE GAME!");
    attroff(A_BOLD | A_BLINK | COLOR_PAIR(108));
    attron(COLOR_PAIR(106));
    for(int i = 0; i < 6; i++) {
        if(i == selected) {
            attron(A_BOLD | A_REVERSE);
        }
        mvprintw(15 + (2*i), 6, "%s", options[i]);
        if(i == selected) {
            attroff(A_BOLD | A_REVERSE);
        }
    }
    if(!user.has_load) {
        attron(A_DIM | A_UNDERLINE);
        mvprintw(17, 7 + strlen(options[1]), "you don't have any saved game to load :(");
        attroff(A_DIM | A_UNDERLINE);
    }
    if(user.is_guest) {
        attron(A_DIM | A_UNDERLINE);
        mvprintw(21, 7 + strlen(options[3]), "you don't have profile :(");
        attroff(A_DIM | A_UNDERLINE);
    }
    attroff(COLOR_PAIR(106));
    refresh();
}

int pre_game_menu() {
    int selected = 0;
    while(1) {
        draw_pre_game_menu_options(selected);
        int c = getch();
        if(c == KEY_UP) {
            if(selected == 0)
                selected = 5;
            else
                selected--;
        }
        if(c == KEY_DOWN) {
            if(selected == 5)
                selected = 0;
            else
                selected++;
        }
        if(c == KEY_ENT) {
            if(selected == 0) {
                return 0;
            }
            if(selected == 1) {
                if(!user.has_load) {
                    continue;
                }
                else {
                    return 1;
                }
            }
            if(selected == 2) {
                settings_menu();
                continue;
            }
            if(selected == 3) {
                if(user.is_guest) {
                    continue;
                }
                else {
                    profile_menu();
                    continue;
                }
            }
            if(selected == 4) {
                scoreboard_menu();
                continue;
            }
            if(selected == 5) {
                return 2;
            }
        }
    }
}

int draw_settings_menu_options(int selected) {
    char options[4][M] = {{"THEME"}, {"DIFFICULTY"}, {"MUSIC"}, {"EXIT"}};
    clear();
    attron(COLOR_PAIR(106));
    draw_menu_border();
    attroff(COLOR_PAIR(106));
    attron(A_BOLD | A_BLINK | COLOR_PAIR(108));
    mvprintw(12, 4, "THE SETTINGS MENU:");
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

int settings_menu() {
    int selected = 0;
    while(1) {
        draw_settings_menu_options(selected);
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
                theme_menu();
                continue;
            }
            if(selected == 1) {
                difficulty_menu();
                continue;
            }
            if(selected == 2) {
                music_menu();
                continue;
            }
            if(selected == 3) {
                return 0;
            }
        }
    }
}

int theme_menu() {
    int selected = user.theme_code;
    while(1) {
        clear();
        attron(COLOR_PAIR(106));
        draw_menu_border();
        attroff(COLOR_PAIR(106));
        attron(A_BOLD | A_BLINK | COLOR_PAIR(108));
        mvprintw(12, 4, "CHOOSE YOUR PLAYER'S COLOR:");
        attroff(A_BOLD | A_BLINK | COLOR_PAIR(108));
        for(int i = 0; i < 3; i++) {
            attron(COLOR_PAIR(color_of_code[i]));
            if(i == selected) {
                attron(A_BOLD | A_REVERSE);
            }
            mvprintw(15 + (2*i), 6, "%s", name_of_code[i]);
            if(i == selected) {
                attroff(A_BOLD | A_REVERSE);
            }
            attroff(COLOR_PAIR(color_of_code[i]));
        }
        refresh();
        int c = getch();
        if(c == KEY_UP) {
            if(selected == 0)
                selected = 2;
            else
                selected--;
        }
        if(c == KEY_DOWN) {
            if(selected == 2)
                selected = 0;
            else
                selected++;
        }
        if(c == KEY_ENT) {
            user.theme_code = selected;
            return 0;
        }
    }
}

int difficulty_menu() {
    int selected = user.difficulty;
    char options[3][M] = {{"EASY"}, {"MEDIUM"}, {"HARD!!!"}};
    while(1) {
        clear();
        attron(COLOR_PAIR(106));
        draw_menu_border();
        attroff(COLOR_PAIR(106));
        attron(A_BOLD | A_BLINK | COLOR_PAIR(108));
        mvprintw(12, 4, "CHOOSE YOUR DIFFICULTY LEVEL:");
        attroff(A_BOLD | A_BLINK | COLOR_PAIR(108));
        attron(COLOR_PAIR(106));
        for(int i = 0; i < 3; i++) {
            if(i == 0)
                attron(COLOR_PAIR(106));
            if(i == 1)
                attron(COLOR_PAIR(108));
            if(i == 2)
                attron(COLOR_PAIR(105));
            if(i == selected) {
                attron(A_BOLD | A_REVERSE);
            }
            mvprintw(15 + (2*i), 6, "%s", options[i]);
            if(i == selected) {
                attroff(A_BOLD | A_REVERSE);
            }
            if(i == 0)
                attroff(COLOR_PAIR(106));
            if(i == 1)
                attroff(COLOR_PAIR(108));
            if(i == 2)
                attroff(COLOR_PAIR(105));
        }
        refresh();
        int c = getch();
        if(c == KEY_UP) {
            if(selected == 0)
                selected = 2;
            else
                selected--;
        }
        if(c == KEY_DOWN) {
            if(selected == 2)
                selected = 0;
            else
                selected++;
        }
        if(c == KEY_ENT) {
            user.difficulty = selected;
            if(user.health > default_health[user.difficulty] || !user.has_load)
                user.health = default_health[user.difficulty];
            return 0;
        }
    }

}

int music_menu() {
    int selected = user.music_stat;
    char options[4][M] = {{"TURN OFF"}, {"PLAY MUSIC1"}, {"PLAY MUSIC2"}, {"EXIT"}};
    while(1) {
        clear();
        attron(COLOR_PAIR(106));
        draw_menu_border();
        attroff(COLOR_PAIR(106));
        attron(A_BOLD | A_BLINK | COLOR_PAIR(108));
        mvprintw(12, 4, "CHOOSE YOUR MUSIC:");
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
        refresh();
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
                char path_prev[M];
                if(user.music_stat == 1)
                    strcpy(path_prev, "music1.mp3");
                else
                    strcpy(path_prev, "music2.mp3");
                Mix_Music *music = Mix_LoadMUS(path_prev);
                Mix_HaltMusic();
                Mix_FreeMusic(music);
                Mix_CloseAudio();
                SDL_Quit();
                user.music_stat = selected;
            }
            else if(selected == 1 || selected == 2) {
                if(user.music_stat == selected)
                    return 0;
                if(user.music_stat > 0) {
                    char path_prev[M];
                    if(user.music_stat == 1)
                        strcpy(path_prev, "music1.mp3");
                    else
                        strcpy(path_prev, "music2.mp3");
                    Mix_Music *music = Mix_LoadMUS(path_prev);
                    Mix_HaltMusic();
                    Mix_FreeMusic(music);
                    Mix_CloseAudio();
                    SDL_Quit();
                }

                SDL_Init(SDL_INIT_AUDIO);
                Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
                char path[M];
                if(selected == 1)
                    strcpy(path, "music1.mp3");
                else
                    strcpy(path, "music2.mp3");
                Mix_Music *music = Mix_LoadMUS(path);
                Mix_PlayMusic(music, -1);
                user.music_stat = selected;
            }
            return 0;
        }
    }


}

int profile_menu() {
    clear();
    attron(COLOR_PAIR(106));
    draw_menu_border();
    attroff(COLOR_PAIR(106));
    attron(A_BOLD | A_BLINK | COLOR_PAIR(108));
    mvprintw(8, (COLS - 20) / 2, "HERE IS YOU PROFILE!");
    attroff(A_BOLD | A_BLINK | COLOR_PAIR(108));attron(A_DIM);
    mvprintw(9, (COLS - 22) / 2, "(press any key to EXIT)");
    attroff(A_DIM);
    for(int i = 0; i < 11; i++) {
        attron(COLOR_PAIR(106));
        for(int j = -40; j < 0; j++) {
            if(i % 2 == 0)
                attron(A_REVERSE);
            mvprintw(11 + i, (COLS / 2) + j, " ");
            if(i % 2 == 0)
                attroff(A_REVERSE);
        }
        attroff(COLOR_PAIR(106));
        attron(COLOR_PAIR(color_of_code[user.theme_code]));
        for(int j = 0; j < 40; j++) {
            if(i % 2 == 1)
                attron(A_REVERSE);
            mvprintw(11 + i, (COLS / 2) + j, " ");
            if(i % 2 == 1)
                attroff(A_REVERSE);
        }
        attroff(COLOR_PAIR(color_of_code[user.theme_code]));
        if(i == 9) {
            attron(COLOR_PAIR(color_of_code[user.theme_code]));
            for(int j = 0; j < 40; j++) {
                attron(A_REVERSE);
                mvprintw(11 + i, (COLS / 2) + j, " ");
                attroff(A_REVERSE);
            }
            attroff(COLOR_PAIR(color_of_code[user.theme_code]));
        }
    }
    char options[11][M] = {{"Username:"}, {"Password:"}, {"Email:"}, {"Points:"}, {"Saved Golds:"}, {"Finished Games:"}, {"EXP:"}, {"Current HP:"}, {"Current Golds:"}, {"Theme Color:"}, {"Difficulty Level:"}};
    for(int i = 0; i < 11; i++) {
        attron(COLOR_PAIR(106));
        if(i % 2 == 0)
            attron(A_REVERSE);
        mvprintw(11 + i, (COLS / 2) - 39, "%s", options[i]);
        if(i % 2 == 0)
            attroff(A_REVERSE);
        attroff(COLOR_PAIR(106));
    }
    attron(COLOR_PAIR(color_of_code[user.theme_code]));
    for(int i = 0; i < 11; i++) {
        if(i % 2 == 1)
            attron(A_REVERSE);
        if(i == 0) {
            mvprintw(11 + i, (COLS / 2) + 1, "%s", user.username);
        }
        if(i == 1) {
            mvprintw(11 + i, (COLS / 2) + 1, "%s", user.email);
        }
        if(i == 2) {
            for(int j = 0; j < strlen(user.password); j++) {
                if(j < 6)
                    mvprintw(11 + i, (COLS / 2) + 1 + j, "*");
                else
                    mvprintw(11 + i, (COLS / 2) + 1 + j, "%c", user.password[j]);
            }
        }
        if(i == 3) {
            mvprintw(11 + i, (COLS / 2) + 1, "%d", user.points);
        }
        if(i == 4) {
            mvprintw(11 + i, (COLS / 2) + 1, "%d", user.all_saved_golds);
        }
        if(i == 5) {
            mvprintw(11 + i, (COLS / 2) + 1, "%d", user.finished_games);
        }
        if(i == 6) {
            time_t now = time(NULL);
            double diffrence_sec = difftime(now, user.first_game_time);
            double diffrence_hrs = diffrence_sec / 3600; 
            mvprintw(11 + i, (COLS / 2) + 1, "%.2lf", diffrence_hrs);
        }
        if(i == 7) {
            mvprintw(11 + i, (COLS / 2) + 1, "%d", user.health);
        }
        if(i == 8) {
            mvprintw(11 + i, (COLS / 2) + 1, "%d", user.golds);
        }
        if(i == 9) {
            mvprintw(11 + i, (COLS / 2) + 1, "%s", name_of_code[user.theme_code]);
        }
        if(i == 10) {
            attroff(COLOR_PAIR(color_of_code[user.theme_code]));
            if(user.difficulty == 0) {
                attron(COLOR_PAIR(106));
                mvprintw(11 + i, (COLS / 2) + 1, "EASY");
                attroff(COLOR_PAIR(106));
            }
            if(user.difficulty == 1) {
                attron(COLOR_PAIR(108));
                mvprintw(11 + i, (COLS / 2) + 1, "MEDIUM");
                attroff(COLOR_PAIR(108));
            }
            if(user.difficulty == 2) {
                attron(COLOR_PAIR(105));
                mvprintw(11 + i, (COLS / 2) + 1, "HARD!!!");
                attroff(COLOR_PAIR(105));
            }
        }
        if(i % 2 == 1)
            attroff(A_REVERSE);
    }
    refresh();
    getch();
}

int get_all_users_scoreboard(USER_S* all_users) {
    USER_S tmp_users[M];

    // user 0
    strcpy(tmp_users[0].username, "neptico");
    tmp_users[0].points = 35;
    tmp_users[0].all_saved_golds = 112;
    tmp_users[0].finished_games = 5;
    tmp_users[0].first_game_time = time(NULL);
    
    // user 1
    strcpy(tmp_users[1].username, "doglas");
    tmp_users[1].points = 20;
    tmp_users[1].all_saved_golds = 50;
    tmp_users[1].finished_games = 4;
    tmp_users[1].first_game_time = time(NULL);
    
    // user 2
    strcpy(tmp_users[2].username, "aliiiii");
    tmp_users[2].points = 65;
    tmp_users[2].all_saved_golds = 12;
    tmp_users[2].finished_games = 6;
    tmp_users[2].first_game_time = time(NULL);
    
    // // user 3
    strcpy(tmp_users[3].username, "bababoy");
    tmp_users[3].points = 4;
    tmp_users[3].all_saved_golds = 0;
    tmp_users[3].finished_games = 10;
    tmp_users[3].first_game_time = time(NULL);
    
    // // user 4
    strcpy(tmp_users[4].username, "GUEST");
    tmp_users[4].points = 100;
    tmp_users[4].all_saved_golds = 9;
    tmp_users[4].finished_games = 2;
    tmp_users[4].first_game_time = time(NULL);
    
    // // user 5
    strcpy(tmp_users[5].username, "majid");
    tmp_users[5].points = 200;
    tmp_users[5].all_saved_golds = 150;
    tmp_users[5].finished_games = 1;
    tmp_users[5].first_game_time = time(NULL);
    
    int n = 6;
    int sort_order[n];
    for(int i = 0; i < n; i++)
        sort_order[i] = i;
    for(int i = 0; i < n; i++) {
        for(int j = i+1; j < n; j++) {
            if(tmp_users[sort_order[i]].points < tmp_users[sort_order[j]].points) {
                int tmp = sort_order[i];
                sort_order[i] = sort_order[j];
                sort_order[j] = tmp;
            }
        }
    }
    for(int i = 0; i < n; i++) {
        strcpy(all_users[i].username, tmp_users[sort_order[i]].username);
        all_users[i].points = tmp_users[sort_order[i]].points;
        all_users[i].all_saved_golds = tmp_users[sort_order[i]].all_saved_golds;
        all_users[i].finished_games = tmp_users[sort_order[i]].finished_games;
        all_users[i].first_game_time = tmp_users[sort_order[i]].first_game_time;
    }
    return n;
}

int scoreboard_menu() {
    clear();
    mvprintw(0, 0, "heee");
    USER_S *all_users = (USER_S*)malloc(M * sizeof(USER_S));;
    int users_count = get_all_users_scoreboard(all_users);
    int lenght = 74;
    clear();
    attron(COLOR_PAIR(106));
    draw_menu_border();
    attroff(COLOR_PAIR(106));
    attron(A_BOLD | A_BLINK | COLOR_PAIR(108));
    mvprintw(8, (COLS - 16) / 2, "THE TOP PLAYERS!");
    attroff(A_BOLD | A_BLINK | COLOR_PAIR(108));
    attron(A_DIM);
    mvprintw(9, (COLS - 22) / 2, "(press any key to EXIT)");
    attroff(A_DIM);
    attron(COLOR_PAIR(106) | A_REVERSE);
    for(int i = 0; i < lenght; i++) {
        mvprintw(11, ((COLS - lenght) / 2) + i, " ");
    }
    int start_from[8] = {-5, -2, 1, 7, 29, 37, 50, 66};
    mvprintw(11, ((COLS - lenght) / 2) + start_from[2], "RANK");
    mvprintw(11, ((COLS - lenght) / 2) + start_from[3] + 6, "Username");
    mvprintw(11, ((COLS - lenght) / 2) + start_from[4], "Points");
    mvprintw(11, ((COLS - lenght) / 2) + start_from[5], "Saved Golds");
    mvprintw(11, ((COLS - lenght) / 2) + start_from[6], "Finished Game");
    mvprintw(11, ((COLS - lenght) / 2) + start_from[7] + 2, "EXP");
    attroff(COLOR_PAIR(106) | A_REVERSE);
    if(users_count > 10)
        users_count = 10;
    for(int i = 0; i < users_count; i++) {
        if(!strcmp(user.username, all_users[i].username)) {
            attron(A_BOLD | A_UNDERLINE);
            for(int j = 0; j < lenght; j++) {
                mvprintw(13 + (2*i), ((COLS - lenght) / 2) + j, " ");
            }
            attroff(A_BOLD | A_UNDERLINE);
            mvprintw(13 + (2*i), ((COLS - lenght) / 2) + start_from[0], "~>");
        }
        if(!strcmp(user.username, all_users[i].username)) {
            attron(A_UNDERLINE);
        }
        if(i == 0) {
            mvprintw(13 + (2*i), ((COLS - lenght) / 2) + start_from[1], "\xF0\x9F\xA5\x87");
            attron(COLOR_PAIR(109) | A_ITALIC);
            attron(A_DIM);
            mvprintw(13 + (2*i), ((COLS - lenght) / 2) + start_from[3] + strlen(all_users[i].username), "(GOAT)");
            attroff(A_DIM);
        }
        if(i == 1) {
            mvprintw(13 + (2*i), ((COLS - lenght) / 2) + start_from[1], "\xF0\x9F\xA5\x88");
            attron(COLOR_PAIR(110) | A_ITALIC);
            attron(A_DIM);
            mvprintw(13 + (2*i), ((COLS - lenght) / 2) + start_from[3] + strlen(all_users[i].username), "(Master)");
            attroff(A_DIM);
        }
        if(i == 2) {
            mvprintw(13 + (2*i), ((COLS - lenght) / 2) + start_from[1], "\xF0\x9F\xA5\x89");
            attron(COLOR_PAIR(111) | A_ITALIC);
            attron(A_DIM);
            mvprintw(13 + (2*i), ((COLS - lenght) / 2) + start_from[3] + strlen(all_users[i].username), "(Warrior)");
            attroff(A_DIM);
        }
        if(i > 2) {
            attron(A_DIM);
        }
        if(!strcmp(user.username, all_users[i].username)) {
            attron(A_UNDERLINE);
        }
        mvprintw(13 + (2*i), ((COLS - lenght) / 2) + start_from[2], "%d", i+1);
        mvprintw(13 + (2*i), ((COLS - lenght) / 2) + start_from[3], "%s", all_users[i].username);
        mvprintw(13 + (2*i), ((COLS - lenght) / 2) + start_from[4], "%d", all_users[i].points);
        mvprintw(13 + (2*i), ((COLS - lenght) / 2) + start_from[5], "%d", all_users[i].all_saved_golds);
        mvprintw(13 + (2*i), ((COLS - lenght) / 2) + start_from[6], "%d", all_users[i].finished_games);
        time_t now = time(NULL);
        double diffrence_sec = difftime(now, user.first_game_time);
        double diffrence_hrs = diffrence_sec / 3600;
        mvprintw(13 + (2*i), ((COLS - lenght) / 2) + start_from[7], "%.2lf", diffrence_hrs);
        if(!strcmp(user.username, all_users[i].username)) {
            attroff(A_UNDERLINE);
        }
        if(i == 0) {
            attroff(COLOR_PAIR(109) | A_ITALIC);
        }
        if(i == 1) {
            attroff(COLOR_PAIR(110) | A_ITALIC);
        }
        if(i == 2) {
            attroff(COLOR_PAIR(111) | A_ITALIC);
        }
        if(i > 2) {
            attroff(A_DIM);
        }
    }
    refresh();
    getch();

    free(all_users);
}
