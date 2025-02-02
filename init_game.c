#include "head.h"

extern USER user;
extern USER_S user_s;

int initialize_new_game();
int generate_levels(int l);
int is_empty(int l, int room_type, int i, int j, int ban_room);
int is_valid_wall(int l, int i, int j);
int is_valid_pillar(int l);
void find_random_empty(int l, int room_type, int type, int ban_room);
int set_pillars(int l);
int set_traps(int l);
int set_spawn(int l);
int set_stair(int l);
int set_golds(int l);
int set_weapoans(int l);
int set_spells(int l);
int set_foods(int l);
int set_enenmies(int l);
int set_doors(int l);
int set_enchant_room();
int set_treasure_room();
int connect_doors(int l, int r1, int r2, int type);
int initialize_load_game();

int is_empty(int l, int room_type, int i, int j, int ban_room) {
    if(user.levels[l].which_room[i][j] == ban_room)
        return 0;
    if(user.levels[l].base[i][j] == '.'
        && user.levels[l].items[i][j] == 0
        && user.levels[l].rooms[user.levels[l].which_room[i][j]].type == room_type) {
            return 1;
        }
    return 0;
}

void find_random_empty(int l, int room_type, int type, int ban_room) {
    POSITION* all_empty = (POSITION*) malloc((N * N) * sizeof(POSITION));
    int n = 0;
    for(int i = 0; i < LINES; i++)
        for(int j = 0; j < COLS; j++)
            if(is_empty(l, room_type, i, j, ban_room) && (type == 0 || is_empty(l, room_type, i, j + 1, ban_room))) {
                all_empty[n].y = i;
                all_empty[n].x = j;
                n++;
            }
    
    int it = rand() % n;
    random_position.y = all_empty[it].y;
    random_position.x = all_empty[it].x;
    free(all_empty);
    return;
}

int is_valid_pillar(int l) {
    for(int i = random_position.y - 1; i <= random_position.y + 1; i++) {
        for(int j = random_position.x - 1; j <= random_position.x + 1; j++) {
            char base = user.levels[l].base[i][j];
            if(base == 'O' || base == '-' || base == '|' || base == '+' || base == '?')
                return 0;  
        }
    }
    return 1;
}

int set_pillars(int l) {
    int n = 2 + (rand() % 3);
    for(int i = 0; i < n; i++) {
        while(1) {
            find_random_empty(l, 0, 0, -1);
            if(!is_valid_pillar(l))
                continue;
            break;
        }
        user.levels[l].base[random_position.y][random_position.x] = 'O';
    }
}

int set_traps(int l) {
    int n = l + 3 + (rand() % 2) + user.difficulty;
    for(int i = 0; i < n; i++) {
        find_random_empty(l, 0, 0, -1);
        user.levels[l].base[random_position.y][random_position.x] = '^';
    }
}

int set_spawn(int l) {
    find_random_empty(l, 0, 0, -1);
    user.levels[l].base[random_position.y][random_position.x] = '>';
    user.levels[l].spawn.y = random_position.y;
    user.levels[l].spawn.x = random_position.x;
}

int set_stair(int l) {
    find_random_empty(l, 0, (l == 3 ? 1 : 0), user.levels[l].which_room[user.levels[l].spawn.y][user.levels[l].spawn.x]);
    user.levels[l].base[random_position.y][random_position.x] = '<';
    if(l == 3)
        user.levels[l].base[random_position.y][random_position.x + 1] = '<';
    user.levels[l].stair.y = random_position.y;
    user.levels[l].stair.x = random_position.x;
}

int set_golds(int l) {
    int n = l + 3 + (rand() % 4);
    for(int i = 0; i < n; i++) {
        find_random_empty(l, 0, 1, -1);
        if(!(rand() % 5)) {
            user.levels[l].items[random_position.y][random_position.x] = 4;
            user.levels[l].items[random_position.y][random_position.x + 1] = 5;
        }
        else {
            user.levels[l].items[random_position.y][random_position.x] = 2;
            user.levels[l].items[random_position.y][random_position.x + 1] = 3;
        }
    }
}

int set_weapoans(int l) {
    if(l != not_sword) {
        find_random_empty(l, 0, 1, -1);
        user.levels[l].items[random_position.y][random_position.x] = 16;
        user.levels[l].items[random_position.y][random_position.x + 1] = 17;
    }
    int n = 1 + (rand() % 3);
    for(int i = 0; i < n; i++) {
        find_random_empty(l, 0, 1, -1);
        user.levels[l].items[random_position.y][random_position.x] = 10;
        user.levels[l].items[random_position.y][random_position.x + 1] = 11;
    }
    n = 1 + (rand() % 3);
    for(int i = 0; i < n; i++) {
        find_random_empty(l, 0, 1, -1);
        user.levels[l].items[random_position.y][random_position.x] = 12;
        user.levels[l].items[random_position.y][random_position.x + 1] = 13;
    }
    n = 1 + (rand() % 3);
    for(int i = 0; i < n; i++) {
        find_random_empty(l, 0, 1, -1);
        user.levels[l].items[random_position.y][random_position.x] = 14;
        user.levels[l].items[random_position.y][random_position.x + 1] = 15;
    }
}

int set_spells(int l) {
    int n = 1 + (rand() % 3);
    for(int i = 0; i < n; i++) {
        find_random_empty(l, 0, 1, -1);
        user.levels[l].items[random_position.y][random_position.x] = 20;
        user.levels[l].items[random_position.y][random_position.x + 1] = 21;
    }
    n = 1 + (rand() % 3);
    for(int i = 0; i < n; i++) {
        find_random_empty(l, 0, 1, -1);
        user.levels[l].items[random_position.y][random_position.x] = 22;
        user.levels[l].items[random_position.y][random_position.x + 1] = 23;
    }
    n = 1 + (rand() % 3);
    for(int i = 0; i < n; i++) {
        find_random_empty(l, 0, 1, -1);
        user.levels[l].items[random_position.y][random_position.x] = 24;
        user.levels[l].items[random_position.y][random_position.x + 1] = 25;
    }
}

int set_foods(int l) {
    int n = 4 + (rand() % 3);
    for(int i = 0; i < n; i++) {
        find_random_empty(l, 0, 1, -1);
        int number, randomx = rand() % 10;
        if(randomx == 0)
            number = 36;
        else if(randomx == 1 || randomx == 2)
            number = 34;
        else if(randomx == 3 || randomx == 4)
            number = 32;
        else
            number = 30;
        user.levels[l].items[random_position.y][random_position.x] = number;
        user.levels[l].items[random_position.y][random_position.x + 1] = number + 1;
    }
}

int set_enemies(int l) {
    int n = 4 + l + (2 * user.difficulty) + (1 - (rand() % 3));
    for(int i = 0; i < n; i++) {
        find_random_empty(l, 0, 0, -1);
        int randomx = rand() % 5;
        int r = user.levels[l].which_room[random_position.y][random_position.x];
        int count = user.levels[l].rooms[r].enemies_count;
        if(randomx == 0) {
            user.levels[l].enemies[random_position.y][random_position.x] = 'D';
            user.levels[l].rooms[r].enemies[count].symbol = 'D';
            user.levels[l].rooms[r].enemies[count].type = 0;
            user.levels[l].rooms[r].enemies[count].health = 5;
            user.levels[l].rooms[r].enemies[count].damage = 4;
        }
        else if(randomx == 1) {
            user.levels[l].enemies[random_position.y][random_position.x] = 'F';
            user.levels[l].rooms[r].enemies[count].symbol = 'F';
            user.levels[l].rooms[r].enemies[count].type = 1;
            user.levels[l].rooms[r].enemies[count].health = 10;
            user.levels[l].rooms[r].enemies[count].damage = 7;
        }
        else if(randomx == 2) {
            user.levels[l].enemies[random_position.y][random_position.x] = 'G';
            user.levels[l].rooms[r].enemies[count].symbol = 'G';
            user.levels[l].rooms[r].enemies[count].type = 2;
            user.levels[l].rooms[r].enemies[count].health = 15;
            user.levels[l].rooms[r].enemies[count].damage = 7;
        }
        else if(randomx == 3) {
            user.levels[l].enemies[random_position.y][random_position.x] = 'S';
            user.levels[l].rooms[r].enemies[count].symbol = 'S';
            user.levels[l].rooms[r].enemies[count].type = 3;
            user.levels[l].rooms[r].enemies[count].health = 20;
            user.levels[l].rooms[r].enemies[count].damage = 11;
        }
        else if(randomx == 4) {
            user.levels[l].enemies[random_position.y][random_position.x] = 'U';
            user.levels[l].rooms[r].enemies[count].symbol = 'U';
            user.levels[l].rooms[r].enemies[count].type = 4;
            user.levels[l].rooms[r].enemies[count].health = 30;
            user.levels[l].rooms[r].enemies[count].damage = 10;
        }
        user.levels[l].rooms[r].enemies[count].pos.y = random_position.y;
        user.levels[l].rooms[r].enemies[count].pos.x = random_position.x;
        user.levels[l].rooms[r].enemies[count].type = randomx;
        user.levels[l].rooms[r].enemies[count].last_seen = -100;
        user.levels[l].rooms[r].enemies_count += 1;

    }

}

int connect_doors(int l, int r1, int r2, int type) {
    POSITION s, t;
    int change[2];
    if(type == 0) {
        s = user.levels[l].rooms[r1].doors[2];
        t = user.levels[l].rooms[r2].doors[0];
        s.y++;
        t.y--;
        change[0] = 1;
        change[1] = (s.x > t.x ? -1 : 1);
    }
    if(type == 1) {
        s = user.levels[l].rooms[r1].doors[1];
        t = user.levels[l].rooms[r2].doors[3];
        s.x++;
        t.x--;
        change[0] = (s.y > t.y ? -1 : 1);
        change[1] = 1;
    }
    if(type == 0 || type == 1) {
        while(s.y != t.y || s.x != t.x) {
            user.levels[l].base[s.y][s.x] = '#';
            if(s.x == t.x) {
                s.y += change[0];
            }
            else if(s.y == t.y) {
                s.x += change[1];
            }
            else {
                if(!(user.levels[l].base[s.y + change[0]][s.x] == ' ' || user.levels[l].base[s.y + change[0]][s.x] == '#'))
                    s.x += change[1];
                else if(!(user.levels[l].base[s.y][s.x + change[1]] == ' ' || user.levels[l].base[s.y][s.x + change[1]] == '#'))
                    s.y += change[0];
                else if(rand() % 2 == 0)
                    s.y += change[0];
                else
                    s.x += change[1];
            }
        }
        user.levels[l].base[s.y][s.x] = '#';
    }
}

int set_doors(int l) {
    for(int k = 0; k < 6; k++) {
        if(k % 2 == 0) {
            int dx = rand() % (user.levels[l].rooms[k].w - 2);
            int y = user.levels[l].rooms[k].corners[3].y;
            int x = user.levels[l].rooms[k].corners[3].x;
            user.levels[l].base[y][x + 1 + dx] = '+';
            user.levels[l].rooms[k].doors[2].y = y;
            user.levels[l].rooms[k].doors[2].x = x + 1 + dx;
        }
        if(k % 2 == 1) {
            int dx = rand() % (user.levels[l].rooms[k].w - 2);
            int y = user.levels[l].rooms[k].corners[0].y;
            int x = user.levels[l].rooms[k].corners[0].x;
            user.levels[l].base[y][x + 1 + dx] = '+';
            user.levels[l].rooms[k].doors[0].y = y;
            user.levels[l].rooms[k].doors[0].x = x + 1 + dx;
        }
    }
    int connection[2] = {rand() % 10, rand() % 10};
    // int connection[2] = {9, 9};

    // connection[0]
    // room 0
    if((connection[0] >= 0 && connection[0] <= 3) || (connection[0] >= 8 && connection[0] <= 9)) {
        int dy = rand() % (user.levels[l].rooms[0].h - 2);
        int y = user.levels[l].rooms[0].corners[1].y;
        int x = user.levels[l].rooms[0].corners[1].x;
        user.levels[l].base[y + 1 + dy][x] = '+';
        user.levels[l].rooms[0].doors[1].y = y + 1 + dy;
        user.levels[l].rooms[0].doors[1].x = x;
    }
    // room 1
    if(connection[0] >= 4 && connection[0] <= 9) {
        int dy = rand() % (user.levels[l].rooms[1].h - 2);
        int y = user.levels[l].rooms[1].corners[1].y;
        int x = user.levels[l].rooms[1].corners[1].x;
        user.levels[l].base[y + 1 + dy][x] = '+';
        user.levels[l].rooms[1].doors[1].y = y + 1 + dy;
        user.levels[l].rooms[1].doors[1].x = x;
    }
    // room 2
    if((connection[0] >= 0 && connection[0] <= 1) || (connection[0] >= 4 && connection[0] <= 5) || (connection[0] >= 8 && connection[0] <= 9)) {
        int dy = rand() % (user.levels[l].rooms[2].h - 2);
        int y = user.levels[l].rooms[2].corners[0].y;
        int x = user.levels[l].rooms[2].corners[0].x;
        user.levels[l].base[y + 1 + dy][x] = '+';
        user.levels[l].rooms[2].doors[3].y = y + 1 + dy;
        user.levels[l].rooms[2].doors[3].x = x;
    }
    // room 3
    if((connection[0] >= 2 && connection[0] <= 3) || (connection[0] >= 6 && connection[0] <= 9)) {
        int dy = rand() % (user.levels[l].rooms[3].h - 2);
        int y = user.levels[l].rooms[3].corners[0].y;
        int x = user.levels[l].rooms[3].corners[0].x;
        user.levels[l].base[y + 1 + dy][x] = '+';
        user.levels[l].rooms[3].doors[3].y = y + 1 + dy;
        user.levels[l].rooms[3].doors[3].x = x;
    }
    
    // connection[1]
    // room 2
    if((connection[1] >= 0 && connection[1] <= 3) || (connection[1] >= 8 && connection[1] <= 9)) {
        int dy = rand() % (user.levels[l].rooms[2].h - 2);
        int y = user.levels[l].rooms[2].corners[1].y;
        int x = user.levels[l].rooms[2].corners[1].x;
        user.levels[l].base[y + 1 + dy][x] = '+';
        user.levels[l].rooms[2].doors[1].y = y + 1 + dy;
        user.levels[l].rooms[2].doors[1].x = x;
    }
    // room 3
    if(connection[1] >= 4 && connection[1] <= 9) {
        int dy = rand() % (user.levels[l].rooms[3].h - 2);
        int y = user.levels[l].rooms[3].corners[1].y;
        int x = user.levels[l].rooms[3].corners[1].x;
        user.levels[l].base[y + 1 + dy][x] = '+';
        user.levels[l].rooms[3].doors[1].y = y + 1 + dy;
        user.levels[l].rooms[3].doors[1].x = x;
    }
    // room 4
    if((connection[1] >= 0 && connection[1] <= 1) || (connection[1] >= 4 && connection[1] <= 5) || (connection[1] >= 8 && connection[1] <= 9)) {
        int dy = rand() % (user.levels[l].rooms[4].h - 2);
        int y = user.levels[l].rooms[4].corners[0].y;
        int x = user.levels[l].rooms[4].corners[0].x;
        user.levels[l].base[y + 1 + dy][x] = '+';
        user.levels[l].rooms[4].doors[3].y = y + 1 + dy;
        user.levels[l].rooms[4].doors[3].x = x;
    }
    // room 5
    if((connection[1] >= 2 && connection[1] <= 3) || (connection[1] >= 6 && connection[1] <= 9)) {
        int dy = rand() % (user.levels[l].rooms[5].h - 2);
        int y = user.levels[l].rooms[5].corners[0].y;
        int x = user.levels[l].rooms[5].corners[0].x;
        user.levels[l].base[y + 1 + dy][x] = '+';
        user.levels[l].rooms[5].doors[3].y = y + 1 + dy;
        user.levels[l].rooms[5].doors[3].x = x;
    }

    // connet doors type 0
    connect_doors(l, 0, 1, 0);
    connect_doors(l, 2, 3, 0);
    connect_doors(l, 4, 5, 0);

    // connect doors type 1
    if(connection[0] == 0 || connection[0] == 1 || connection[0] == 8)
        connect_doors(l, 0, 2, 1);
    if(connection[0] == 2 || connection[0] == 3 || connection[0] == 9)
        connect_doors(l, 0, 3, 1);
    if(connection[0] == 4 || connection[0] == 5 || connection[0] == 9)
        connect_doors(l, 1, 2, 1);
    if(connection[0] == 6 || connection[0] == 7 || connection[0] == 8)
        connect_doors(l, 1, 3, 1);

    if(connection[1] == 0 || connection[1] == 1 || connection[1] == 8)
        connect_doors(l, 2, 4, 1);
    if(connection[1] == 2 || connection[1] == 3 || connection[1] == 9)
        connect_doors(l, 2, 5, 1);
    if(connection[1] == 4 || connection[1] == 5 || connection[1] == 9)
        connect_doors(l, 3, 4, 1);
    if(connection[1] == 6 || connection[1] == 7 || connection[1] == 8)
        connect_doors(l, 3, 5, 1);
}

int is_valid_wall(int l, int i, int j) {
    if(user.levels[l].base[i][j] != '|' && user.levels[l].base[i][j] != '-')
        return 0;
    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type != 0)
        return 0;
    if(user.levels[l].base[i][j] == '-' && (user.levels[l].base[i][j - 1] != '-' || user.levels[l].base[i][j + 1] != '-'))
        return 0;
    return 1;
}

int set_enchant_room() {
    int l = rand() % 3;
    user.levels[l].has_enchant_room = 1;
    int h = 6 + (rand() % (LINES - 25));
    int w = 6 + (rand() % 3);
    int y = 3 + (rand() % (LINES - 15 - h));
    int x = COLS - 15 + (rand() % 3);

    user.levels[l].rooms[6].visited = 0;
    user.levels[l].rooms[6].type = 1;

    user.levels[l].rooms[6].h = h;
    user.levels[l].rooms[6].w = w;

    user.levels[l].rooms[6].corners[0].y = y;
    user.levels[l].rooms[6].corners[0].x = x;

    user.levels[l].rooms[6].corners[1].y = y;
    user.levels[l].rooms[6].corners[1].x = x + w;

    user.levels[l].rooms[6].corners[2].y = y + h;
    user.levels[l].rooms[6].corners[2].x = x + w;

    user.levels[l].rooms[6].corners[3].y = y + h;
    user.levels[l].rooms[6].corners[3].x = x;

    for(int i = y; i <= y + h; i++) {
        for(int j = x; j <= x + w; j++) {
            user.levels[l].which_room[i][j] = 6;
            if(i == y || i == y + h)
                user.levels[l].base[i][j] = '-';
            else if(j == x || j == x + w)
                user.levels[l].base[i][j] = '|';
            else
                user.levels[l].base[i][j] = '.';
        }
    }

    int door_side = rand() % 4;
    if(door_side == 0) {
        int r = rand() % (w - 2);
        user.levels[l].base[y][x + 1 + r] = '?';
        user.levels[l].rooms[6].doors[0].y = y;
        user.levels[l].rooms[6].doors[0].x = x + 1 + r;
        user.levels[l].secret_doors[1].y = y;
        user.levels[l].secret_doors[1].x = x + 1 + r;
    }
    if(door_side == 1) {
        int r = rand() % (h - 2);
        user.levels[l].base[y + 1 + r][x + w] = '?';
        user.levels[l].rooms[6].doors[1].y = y + 1 + r;
        user.levels[l].rooms[6].doors[1].x = x + w;
        user.levels[l].secret_doors[1].y = y + 1 + r;
        user.levels[l].secret_doors[1].x = x + w;
    }
    if(door_side == 2) {
        int r = rand() % (w - 2);
        user.levels[l].base[y + h][x + 1 + r] = '?';
        user.levels[l].rooms[6].doors[2].y = y + h;
        user.levels[l].rooms[6].doors[2].x = x + 1 + r;
        user.levels[l].secret_doors[1].y = y + h;
        user.levels[l].secret_doors[1].x = x + 1 + r;
    }
    if(door_side == 3) {
        int r = rand() % (h - 2);
        user.levels[l].base[y + 1 + r][x] = '?';
        user.levels[l].rooms[6].doors[3].y = y + 1 + r;
        user.levels[l].rooms[6].doors[3].x = x;
        user.levels[l].secret_doors[1].y = y + 1 + r;
        user.levels[l].secret_doors[1].x = x;
    }
    POSITION* all_empty = (POSITION*) malloc((N * N) * sizeof(POSITION));
    int n = 0;
    for(int i = 0; i < LINES; i++)
        for(int j = 0; j < COLS; j++)
            if(is_valid_wall(l, i, j)) {
                all_empty[n].y = i;
                all_empty[n].x = j;
                n++;
            }
    int it = rand() % n;
    user.levels[l].base[all_empty[it].y][all_empty[it].x] = '?';
    user.levels[l].secret_doors[0].y = all_empty[it].y;
    user.levels[l].secret_doors[0].x = all_empty[it].x;
    free(all_empty);

    n = ((w-2) * (h-2)) / 5 + 3  - (rand() % 5);
    for(int i = 0; i < n; i++) {
        find_random_empty(l, 1, 1, -1);
        int number = rand() % 3;
        user.levels[l].items[random_position.y][random_position.x] = 20 + (2 * number);
        user.levels[l].items[random_position.y][random_position.x + 1] = 21 + (2 * number);
    }
}

int set_treasure_room() {
    // default value
    user.levels[4].has_enchant_room = 0;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            user.levels[4].which_room[i][j] = -1;
            user.levels[4].visited[i][j] = 0;
            user.levels[4].items[i][j] = 0;
            user.levels[4].base[i][j] = ' ';
            user.levels[4].trown[i][j].existence = 0;
            user.levels[4].trown[i][j].count[0] = 0;
            user.levels[4].trown[i][j].count[1] = 0;
            user.levels[4].trown[i][j].count[2] = 0;
        }
    }

    int up = LINES/2 - 8 - (rand() % 5);
    int down = LINES/2 + 8 + (rand() % 5);
    int left = COLS/2 - 20 - (rand() % 12);
    int right = COLS/2 + 20 + (rand() % 12);

    user.levels[4].rooms[0].visited = 0;
    user.levels[4].rooms[0].enemies_count = 0;
    user.levels[4].rooms[0].type = 2;

    user.levels[4].rooms[0].h = down - up;
    user.levels[4].rooms[0].w = right - left;

    user.levels[4].rooms[0].corners[0].y = up;
    user.levels[4].rooms[0].corners[0].x = left;

    user.levels[4].rooms[0].corners[1].y = up;
    user.levels[4].rooms[0].corners[1].x = right;

    user.levels[4].rooms[0].corners[2].y = down;
    user.levels[4].rooms[0].corners[2].x = right;

    user.levels[4].rooms[0].corners[3].y = down;
    user.levels[4].rooms[0].corners[3].x = left;

    for(int i = up; i <= down; i++) {
        for(int j = left; j <= right; j++) {
            user.levels[4].which_room[i][j] = 0;
            if(i == up || i == down)
                user.levels[4].base[i][j] = '-';
            else if(j == left || j == right)
                user.levels[4].base[i][j] = '|';
            else
                user.levels[4].base[i][j] = '.';
        }
    }

    // set spawn
    find_random_empty(4, 2, 0, -1);
    user.levels[4].base[random_position.y][random_position.x] = '>';
    user.levels[4].spawn.y = random_position.y;
    user.levels[4].spawn.x = random_position.x;

    // set stair
    find_random_empty(4, 2, 0, -1);
    user.levels[4].base[random_position.y][random_position.x] = '<';
    user.levels[4].stair.y = random_position.y;
    user.levels[4].stair.x = random_position.x;

    // set golds
    int n = 20 + (rand() % 20);
    for(int i = 0; i < n; i++) {
        find_random_empty(4, 2, 1, -1);
        if(!(rand() % 5)) {
            user.levels[4].items[random_position.y][random_position.x] = 4;
            user.levels[4].items[random_position.y][random_position.x + 1] = 5;
        }
        else {
            user.levels[4].items[random_position.y][random_position.x] = 2;
            user.levels[4].items[random_position.y][random_position.x + 1] = 3;
        }
    }

    // set traps
    n = 15 + (rand() % 10);
    for(int i = 0; i < n; i++) {
        find_random_empty(4, 2, 0, -1);
        user.levels[4].base[random_position.y][random_position.x] = '^';
    }

    // set enemies
    
    n = 10 + (2 * user.difficulty) + (1 - (rand() % 3));
    for(int i = 0; i < n; i++) {
        find_random_empty(4, 2, 0, -1);
        int randomx = rand() % 5;
        int r = user.levels[4].which_room[random_position.y][random_position.x];
        int count = user.levels[4].rooms[r].enemies_count;
        if(randomx == 0) {
            user.levels[4].enemies[random_position.y][random_position.x] = 'D';
            user.levels[4].rooms[r].enemies[count].symbol = 'D';
            user.levels[4].rooms[r].enemies[count].health = 5;
            user.levels[4].rooms[r].enemies[count].damage = 4;
        }
        else if(randomx == 1) {
            user.levels[4].enemies[random_position.y][random_position.x] = 'F';
            user.levels[4].rooms[r].enemies[count].symbol = 'F';
            user.levels[4].rooms[r].enemies[count].health = 10;
            user.levels[4].rooms[r].enemies[count].damage = 7;
        }
        else if(randomx == 2) {
            user.levels[4].enemies[random_position.y][random_position.x] = 'G';
            user.levels[4].rooms[r].enemies[count].symbol = 'G';
            user.levels[4].rooms[r].enemies[count].health = 15;
            user.levels[4].rooms[r].enemies[count].damage = 7;
        }
        else if(randomx == 3) {
            user.levels[4].enemies[random_position.y][random_position.x] = 'S';
            user.levels[4].rooms[r].enemies[count].symbol = 'S';
            user.levels[4].rooms[r].enemies[count].health = 20;
            user.levels[4].rooms[r].enemies[count].damage = 11;
        }
        else if(randomx == 4) {
            user.levels[4].enemies[random_position.y][random_position.x] = 'U';
            user.levels[4].rooms[r].enemies[count].symbol = 'U';
            user.levels[4].rooms[r].enemies[count].health = 30;
            user.levels[4].rooms[r].enemies[count].damage = 10;
        }
        user.levels[4].rooms[r].enemies[count].pos.y = random_position.y;
        user.levels[4].rooms[r].enemies[count].pos.x = random_position.x;
        user.levels[4].rooms[r].enemies[count].type = randomx;
        user.levels[4].rooms[r].enemies[count].last_seen = 0;
        user.levels[4].rooms[r].enemies_count += 1;

    }
}

int generate_levels(int l) {
    // default value
    user.levels[l].has_enchant_room = 0;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            user.levels[l].which_room[i][j] = -1;
            user.levels[l].visited[i][j] = 0;
            user.levels[l].items[i][j] = 0;
            user.levels[l].enemies[i][j] = 0;
            user.levels[l].base[i][j] = ' ';
            user.levels[l].trown[i][j].existence = 0;
            user.levels[l].trown[i][j].count[0] = 0;
            user.levels[l].trown[i][j].count[1] = 0;
            user.levels[l].trown[i][j].count[2] = 0;
        }
    }

    int max_h = (LINES - 3) / 2;
    int h_lines[3] = {1, 1 + max_h, 1 + (2*max_h)};
    int max_w = (COLS - 18)/3;
    int v_lines[4] = {0, max_w, 2 * max_w, 3 * max_w};
    int h, w, y, x;
    for(int k = 0; k < 6; k++) {
        h = 6 + (rand() % (max_h - 10));
        w = 8 + (rand() % (max_w - 12));
        y = h_lines[k%2] + 2 + (rand() % (max_h - 4 - h));
        x = v_lines[k/2] + 2 + (rand() % (max_w - 4 - w));

        user.levels[l].rooms[k].visited = 0;
        user.levels[l].rooms[k].enemies_count = 0;
        user.levels[l].rooms[k].type = 0;

        user.levels[l].rooms[k].h = h;
        user.levels[l].rooms[k].w = w;

        user.levels[l].rooms[k].corners[0].y = y;
        user.levels[l].rooms[k].corners[0].x = x;

        user.levels[l].rooms[k].corners[1].y = y;
        user.levels[l].rooms[k].corners[1].x = x + w;

        user.levels[l].rooms[k].corners[2].y = y + h;
        user.levels[l].rooms[k].corners[2].x = x + w;

        user.levels[l].rooms[k].corners[3].y = y + h;
        user.levels[l].rooms[k].corners[3].x = x;

        for(int i = y; i <= y + h; i++) {
            for(int j = x; j <= x + w; j++) {
                user.levels[l].which_room[i][j] = k;
                if(i == y || i == y + h)
                    user.levels[l].base[i][j] = '-';
                else if(j == x || j == x + w)
                    user.levels[l].base[i][j] = '|';
                else
                    user.levels[l].base[i][j] = '.';
            }
        }
    }
    set_pillars(l);
    set_traps(l);
    set_doors(l);
    set_spawn(l);
    set_stair(l);

    set_golds(l);
    not_sword = rand() % 4;
    set_weapoans(l);
    set_spells(l);
    set_foods(l);
    set_enemies(l);
}

int initialize_new_game() {
    user.timer = 0;
    user.health = default_health[user.difficulty];
    user.hunger = default_hunger;
    user.golds = 0;
    user.last_attack = -100;
    user.last_meal = 0;
    user.current_level = 0;
        
    user.weapoans[0].count = 1; user.weapoans[0].damage = 5; user.weapoans[0].range = 0; user.weapoans[0].collect = 0;
    user.weapoans[1].count = 0; user.weapoans[1].damage = 12; user.weapoans[1].range = 5; user.weapoans[1].collect = 10;
    user.weapoans[2].count = 0; user.weapoans[2].damage = 15; user.weapoans[2].range = 10; user.weapoans[2].collect = 8;
    user.weapoans[3].count = 0; user.weapoans[3].damage = 5; user.weapoans[3].range = 5; user.weapoans[3].collect = 20;
    user.weapoans[4].count = 0; user.weapoans[4].damage = 10; user.weapoans[4].range = 0; user.weapoans[4].collect = 1;
    user.selected_weapoan = 0;

    user.spells[0].count = 0; user.spells[0].last_time = -100;
    user.spells[1].count = 0; user.spells[1].last_time = -100;
    user.spells[2].count = 0; user.spells[2].last_time = -100;

    user.foods[0].type = -1;
    user.foods[1].type = -1;
    user.foods[2].type = -1;
    user.foods[3].type = -1;
    user.foods[4].type = -1;
    user.foods_count = 0;

    for(int k = 0; k < 4; k++) {
        generate_levels(k);
    }
    set_enchant_room();
    set_treasure_room();
    user.pos.y = user.levels[0].spawn.y;
    user.pos.x = user.levels[0].spawn.x;
    user.levels[0].visited[user.pos.y][user.pos.x] = 1;
    user.levels[0].rooms[user.levels[0].which_room[user.pos.y][user.pos.x]].visited = 1;
}

int initialize_load_game() {
    if(user.health > default_health[user.difficulty])
        user.health = default_health[user.difficulty];
}