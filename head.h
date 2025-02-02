#ifndef HEAD_H
#define HEAD_H
#include <stdio.h>
#include <ncursesw/ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <locale.h>
#include <wchar.h>

#define M 50
#define N 200
#define KEY_ENT 10
#define KEY_ESC 27

typedef struct {
    int x;
    int y;
} POSITION;

typedef struct {
    char existence;
    char count[3];
} TROWN;

typedef struct {
    POSITION pos;
    char symbol;
    int type;
    int damage, health;
    int last_seen;
} ENEMY;

typedef struct {
    int damage, range;
    int collect;
    int count;
} WEAPOAN;

typedef struct {
    int count;
    int last_time;
} SPELL;

typedef struct {
    // -1: empty
    // 0: +10 health, +20 hunger;
    // 1: +10 health, +20 hunger, damage(2) spell;
    // 2: +10 health, +20 hunger, speed(1) speel;
    // 3: -10 health, +10 hunger;
    int type;
    int collect_time;
} FOOD;

typedef struct {
    POSITION corners[4], doors[4];
    ENEMY enemies[M]; int enemies_count;
    int h, w;
    int type;
    int visited;
} ROOM;

typedef struct {
    ROOM rooms[7];
    char which_room[N][N];
    char visited[N][N];
    char items[N][N];
    char enemies[N][N];
    char base[N][N];
    TROWN trown[N][N];
    POSITION spawn, stair, secret_doors[2];

    int has_enchant_room;
} LEVEL;

typedef struct {
    char username[M], email[M], password[M];
    int is_guest, has_load;
    int difficulty;
    int all_saved_golds, points;
    time_t first_game_time;
    int theme_code;
    int finished_games;
    int music_stat;
    
    int timer;
    int health, hunger, golds;
    POSITION pos;
    WEAPOAN weapoans[5]; int selected_weapoan;
    SPELL spells[3];
    FOOD foods[5]; int foods_count;
    int last_attack, last_meal;

    int current_level;
    LEVEL levels[5];
} USER;

typedef struct {
    char username[M];
    int all_saved_golds, points;
    time_t first_game_time;
    int finished_games;
} USER_S;

int default_health[3] = {80, 60, 40};
int default_hunger = 100;
int color_of_code[3] = {107, 102, 104};
char name_of_code[3][M] = {{"WHITE"}, {"CYAN"}, {"MAGENTA"}};
char weapoan_names[5][M] = {{"Mace\U0001F528"}, {"Dagger\U0001F52A"}, {"Magic Wand\U0001F9F9"}, {"Arrow\U0001F3F9"}, {"Sword\U00002694"}};
char spell_names[3][M] = {{"Health\U0001FA79"}, {"Speed\U000026A1"}, {"Damage\U0001F4A5"}};
char food_names[4][M] = {{"Normal Food\U0001F35E"}, {"Fine Food\U0001F356"}, {"Magic Food\U0001F347"}, {"Corrupted Food\U0001F9A0"}};
char enemies_names[5][M] = {{"Daemon"}, {"Fire Breath"}, {"Giant"}, {"Snake"}, {"Undead"}};
POSITION random_position;
int not_sword;
char messages_log[M][150], messages_count;

#endif