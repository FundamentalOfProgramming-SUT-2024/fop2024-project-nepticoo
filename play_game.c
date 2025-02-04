#include "head.h"

extern USER user;
extern USER_S user_s;

int play_game();
int print_map();
int print_map_info();
int show_messages();
int handle_movement(int c);
int is_move_key(int c);
int is_special_key(int c);
int pick_up_item();
int apply_attack();
int move_enemies();
int update_visited(int l, int y, int x);
int update_hunger();
int update_health();
int update_food();
int update_music(int room_type);
int food_menu();
int weopoan_menu();
int spell_menu();
int map_menu();

int play_game() {
    int c, f_pressed = 0, g_pressed = 0;
    messages_count = 0;
    while(1) {
        print_map();
        show_messages();
        if(!f_pressed)
            c = getch();
        int move_type = handle_movement(c);
        if(move_type != 0) {
            f_pressed = 0;
            g_pressed = 0;
        }
        if(move_type == -1) {
            continue;
        }
        else if(move_type == 0) {
            update_visited(user.current_level, user.pos.y, user.pos.x);
            int r = user.levels[user.current_level].which_room[user.pos.y][user.pos.x];
            update_music(user.levels[user.current_level].rooms[r].type);
            if(!g_pressed && !(r != -1 && user.levels[user.current_level].rooms[r].type == 3))
                pick_up_item();
            g_pressed = 0;

            int speed_spell_passed = user.timer - user.spells[1].last_time;
            if(speed_spell_passed >= 10 || speed_spell_passed % 2 == 1)
                move_enemies();
            update_hunger();
            update_health();
            update_food();
            if(user.health <= 0) {
                return 1;
            }

            user.timer++;
        }
        else if(move_type == 1) {
            // attack !!!
            if(apply_attack() == -1) {
                continue;
            }

            int speed_spell_passed = user.timer - user.spells[1].last_time;
            if(speed_spell_passed >= 10 || speed_spell_passed % 2 == 1)
                move_enemies();
            update_hunger();
            update_health();
            update_food();
            if(user.health <= 0) {
                return 1;
            }

            user.timer++;
        }
        else if(move_type == 2) {
            continue;
        }
        else if(move_type == 3){
            f_pressed = 1;
            c = getch();
        }
        else if(move_type == 4) {
            g_pressed = 1;
        }
        else {
            return move_type - 5;
        }
    }
}

int print_map() {
    clear();
    int l = user.current_level;

    // print base
    for(int i = 0; i < LINES; i++) {
        for(int j = 0; j < COLS; j++) {
            int base = user.levels[l].base[i][j];
            int visit_self = user.levels[l].visited[i][j], visit_room = 0;
            if(user.levels[l].which_room[i][j] != -1)
                visit_room = user.levels[l].rooms[user.levels[l].which_room[i][j]].visited;
            
            if(user.levels[l].which_room[i][j] != -1 && user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 1)
                attron(COLOR_PAIR(103));
            if(user.levels[l].which_room[i][j] != -1 && user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 2)
                attron(COLOR_PAIR(109));
            if(user.levels[l].which_room[i][j] != -1 && user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 3)
                attron(COLOR_PAIR(101));
            if(user.levels[l].has_enchant_room && user.levels[l].secret_doors[0].y == i && user.levels[l].secret_doors[0].x == j && user.levels[l].visited[user.levels[l].secret_doors[1].y][user.levels[l].secret_doors[1].x] == 0)
                attron(A_BLINK);
            if(base == ' ') {
                mvprintw(i, j, " ");
            }
            else if(base == '#') {
                int show = 0;
                for(int dy = -1; dy <= 1; dy++) {
                    for(int dx = -1; dx <= 1; dx++) {
                        if(i + dy < 0 || i + dy >= LINES || j + dx < 0 || j + dx >= COLS)
                            continue;
                        if(user.levels[l].visited[i + dy][j + dx] == 1)
                            show = 1;
                    }
                }
                if(show)
                    mvprintw(i, j, "%c", base);
                else
                    mvprintw(i, j, " ");
            }
            else if(base == '+') {
                if(visit_room == 1) {
                    mvprintw(i, j, "%c", base);
                    continue;
                }
                int show = 0;
                for(int dy = -1; dy <= 1; dy++) {
                    for(int dx = -1; dx <= 1; dx++) {
                        if(i + dy < 0 || i + dy >= LINES || j + dx < 0 || j + dx >= COLS)
                            continue;
                        if(user.levels[l].visited[i + dy][j + dx] == 1)
                            show = 1;
                    }
                }
                if(show)
                    mvprintw(i, j, "%c", base);
                else
                    mvprintw(i, j, " ");
                
            }
            else if(base == '.' || base == '|' || base == '-' || base == 'O' || base == '>' || base == '<') {
                if(base == '.' && user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 3) {
                    if(abs(user.pos.y - i) > 2 || abs(user.pos.x - j) > 2)
                        continue;
                }
                if(visit_room == 1)
                    mvprintw(i, j, "%c", base);
                else
                    mvprintw(i, j, " ");
            }
            else if(base == '?' || base == '^') {
                if(visit_self == 1)
                    mvprintw(i, j, "%c", base);
                else {
                    if(base == '^') {
                        if(visit_room == 0) {
                            mvprintw(i, j, " ");
                        }
                        else {
                            mvprintw(i, j, ".");
                        }
                        
                    }
                    else {
                        if(visit_room == 0) {
                            mvprintw(i, j, " ");
                        }
                        else if(user.levels[l].base[i][j+1] == '-' || user.levels[l].base[i][j+1] == '+') {
                            mvprintw(i, j, "-");
                        }
                        else{
                            mvprintw(i, j, "|");
                        }
                    }
                }
            }

            if(user.levels[l].which_room[i][j] != -1 && user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 1)
                attroff(COLOR_PAIR(103));
            if(user.levels[l].which_room[i][j] != -1 && user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 2)
                attroff(COLOR_PAIR(109));
            if(user.levels[l].which_room[i][j] != -1 && user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 3)
                attroff(COLOR_PAIR(101));
            if(user.levels[l].has_enchant_room && user.levels[l].secret_doors[0].y == i && user.levels[l].secret_doors[0].x == j && user.levels[l].visited[user.levels[l].secret_doors[1].y][user.levels[l].secret_doors[1].x] == 0)
                attroff(A_BLINK);
        }   
    }

    // print items
    for(int i = 0; i < LINES; i++) {
        for(int j = 0; j < COLS; j++) {
            if(user.levels[l].which_room[i][j] != -1 && user.levels[l].rooms[user.levels[l].which_room[i][j]].visited == 1) {
                if(user.levels[l].items[i][j] != 0) {
                    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 3) {
                        if(abs(user.pos.y - i) > 2 || abs(user.pos.x - j) > 2)
                            continue;
                    }
                    switch(user.levels[l].items[i][j]) {
                        case(2): {
                            // gold
                            mvprintw(i, j, "\U0001F4B0");
                            break;
                        }
                        case(4): {
                            // black gold
                            mvprintw(i, j, "\U0001F48E");
                            break;
                        }
                        case(10): {
                            // dagger
                            mvprintw(i, j, "\U0001F52A");
                            break;
                        }
                        
                        case(12): {
                            // magic wand
                            mvprintw(i, j, "\U0001F9F9");
                            break;
                        }
                        case(14): {
                            // arrow
                            mvprintw(i, j, "\U0001F3F9");
                            break;
                        }
                        case(16): {
                            // sword
                            mvprintw(i, j, "\U00002694");
                            break;
                        }
                        case(20): {
                            // health spell
                            mvprintw(i, j, "\U0001FA79");
                            break;
                        }
                        case(22): {
                            // speed spell
                            mvprintw(i, j, "\U000026A1");
                            break;
                        }
                        case(24): {
                            // damage spell
                            mvprintw(i, j, "\U0001F4A5");
                            break;
                        }
                        case(30): {
                            // normal food
                            mvprintw(i, j, "\U0001F35E");
                            break;
                        }
                        case(32): {
                            // fine food
                            mvprintw(i, j, "\U0001F356");
                            break;
                        }
                        case(34): {
                            // magic food
                            mvprintw(i, j, "\U0001F347");
                            break;
                        }
                        case(36): {
                            // corrupted food
                            mvprintw(i, j, "\U0001F35E");
                            break;
                        }
                    }
                }
                if(user.levels[l].base[i][j] == '<' && user.levels[l].base[i][j+1] == '<') {
                    // treasure
                    mvprintw(i, j, "\U0001F3C6");
                }
            }
        }
    }

    // print trown_weapoan
    for(int i = 0; i < LINES; i++) {
        for(int j = 0; j < COLS; j++) {
            if(user.levels[l].which_room[i][j] != -1 && user.levels[l].rooms[user.levels[l].which_room[i][j]].visited == 1) {
                if(user.levels[l].trown[i][j].existence == 1) {
                    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 1) {
                        attron(COLOR_PAIR(103));
                    }
                    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 2) {
                        attron(COLOR_PAIR(109));
                    }
                    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 3) {
                        if(abs(user.pos.y - i) > 2 || abs(user.pos.x - j) > 2)
                            continue;
                        attron(COLOR_PAIR(101));
                    }
                    mvprintw(i, j, "\u27B6");
                    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 1) {
                        attroff(COLOR_PAIR(103));
                    }
                    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 2) {
                        attroff(COLOR_PAIR(109));
                    }
                    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 3)
                        attroff(COLOR_PAIR(101));
                }
            }
        }
    }
    // print enemies
    for(int i = 0; i < LINES; i++) {
        for(int j = 0; j < COLS; j++) {
            if(user.levels[l].which_room[i][j] != -1 && user.levels[l].rooms[user.levels[l].which_room[i][j]].visited == 1) {
                if(user.levels[l].enemies[i][j] != 0) {
                    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 1) {
                        attron(COLOR_PAIR(103));
                    }
                    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 2) {
                        attron(COLOR_PAIR(109));
                    }
                    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 3) {
                        if(abs(user.pos.y - i) > 2 || abs(user.pos.x - j) > 2)
                            continue;
                        attron(COLOR_PAIR(101));
                    }
                    mvprintw(i, j, "%c", user.levels[l].enemies[i][j]);
                    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 1) {
                        attroff(COLOR_PAIR(103));
                    }
                    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 2) {
                        attroff(COLOR_PAIR(109));
                    }
                    if(user.levels[l].rooms[user.levels[l].which_room[i][j]].type == 3)
                        attroff(COLOR_PAIR(101));
                }
            }
        }
    }
    
    // print user
    attron(COLOR_PAIR(color_of_code[user.theme_code]));
    if(user.levels[l].items[user.pos.y][user.pos.x] != 0) {
        if(user.levels[l].items[user.pos.y][user.pos.x] % 2 == 0) {
            mvprintw(user.pos.y, user.pos.x, "@.");
        }
        else {
            mvprintw(user.pos.y, user.pos.x-1, ".@");
        }
    }
    else if(user.levels[l].base[user.pos.y][user.pos.x] == '<') {
        if(user.levels[l].base[user.pos.y][user.pos.x+1] == '<') {
            mvprintw(user.pos.y, user.pos.x, "@<");
        }
        else if(user.levels[l].base[user.pos.y][user.pos.x-1] == '<') {
            mvprintw(user.pos.y, user.pos.x-1, "<@");
        }
        else {
            mvprintw(user.pos.y, user.pos.x, "@");
        }
    }
    else {
        mvprintw(user.pos.y, user.pos.x, "@");
    }
    attroff(COLOR_PAIR(color_of_code[user.theme_code]));

    if(user.levels[l].has_enchant_room)
        mvprintw(0, 0, "YES");

    // print map info
    print_map_info();
    refresh();
}

int print_map_info() {
    int l = user.current_level;
    attron(A_BOLD);
    mvprintw(0, 0, "Messages:");
    attron(A_DIM);
    mvprintw(0, 10, "(press ENTER to SKIP)");
    attroff(A_DIM);
    mvprintw(0, COLS/2 - 3, "LVL: %d", l + 1);
    attron(COLOR_PAIR(color_of_code[user.theme_code]));
    mvprintw(0, COLS - 3 - strlen(user.username), "-%s-", user.username);
    attroff(COLOR_PAIR(color_of_code[user.theme_code]));
    
    int lenght = (COLS - 2) / 4;
    attron(A_UNDERLINE);
    mvprintw(LINES - 3, 1 + lenght * 0 + 10, "Health:");
    mvprintw(LINES - 3, 1 + lenght * 1 + 10, "Hunger:");
    mvprintw(LINES - 3, 1 + lenght * 2 + 10, "Golds:");
    mvprintw(LINES - 3, 1 + lenght * 3 + 10, "Selected Weapoan:"); 
    attroff(A_UNDERLINE);
    mvprintw(LINES - 2, 2 + lenght * 0 + 10, "%d/%d", user.health, default_health[user.difficulty]);
    mvprintw(LINES - 2, 2 + lenght * 1 + 10, "%d/%d", user.hunger, default_hunger);
    mvprintw(LINES - 2, 2 + lenght * 2 + 10, "%d", user.golds);
    mvprintw(LINES - 2, 2 + lenght * 3 + 10, "%s", weapoan_names[user.selected_weapoan]);
    attroff(A_BOLD);
}

int show_messages() {
    timeout(2000);
    for(int i = 0; i < messages_count; i++) {
        mvprintw(1, 4, "%s ", messages_log[i]);
        if(i != messages_count - 1) {
            attron(A_DIM | A_REVERSE);
            printw("MORE");
            attroff(A_DIM | A_REVERSE);
        }
        refresh();
        int c = getch();
        for(int j = 0; j < 100; j++)
            mvprintw(1, j, " ");
        refresh();
        if(c == KEY_ENT) {
            break;
        }
    }
    messages_count = 0;
    timeout(-1);

}

int is_move_key(int c) {
    if(c == 'h' || c == 'j' || c == 'k' || c == 'l' || c == 'y' || c == 'u' || c == 'b' || c == 'n')
        return 1;
    if(c == 'H' || c == 'J' || c == 'K' || c == 'L' || c == 'Y' || c == 'U' || c == 'B' || c == 'N')
        return 1;
    return 0;
}

int is_special_key(int c) {
    if(c == 'e' || c == 'i' || c == 'p' || c == 'm' || c == 's' || c == '.' || c == ',' || c == '/' || c == 'f' || c == 'g' || c == 'q')
        return 1;
    if(c == 'E' || c == 'I' || c == 'P' || c == 'M' || c == 'S' || c == '>' || c == '<' || c == '?' || c == 'F' || c == 'G' || c == 'Q')
        return 1;
    return 0;
}

int handle_movement(int c) {
    if(is_move_key(c)) {
        int dy = 0, dx = 0;
        if(c == 'h' || c == 'H')
            dx = -1;
        if(c == 'j' || c == 'J')
            dy = -1;
        if(c == 'k' || c == 'K')
            dy = 1;
        if(c == 'l' || c == 'L')
            dx = 1;
        if(c == 'y' || c == 'Y') {
            dy = -1;
            dx = -1;
        }
        if(c == 'u' || c == 'U') {
            dy = -1;
            dx = 1;
        }
        if(c == 'b' || c == 'B') {
            dy = 1;
            dx = -1;
        }
        if(c == 'n' || c == 'N') {
            dy = 1;
            dx = 1;
        }
        int next = user.levels[user.current_level].base[user.pos.y + dy][user.pos.x + dx];
        if(next == '.' || next == '#' || next == '+' || next == '?' || next == '^' || next == '>' || next == '<') {
            if(user.levels[user.current_level].enemies[user.pos.y + dy][user.pos.x + dx] != 0)
                return -1;
            else {
                user.pos.y += dy;
                user.pos.x += dx;
                return 0;
            }
        }
        return -1;
    }
    if(c == ' ') {
        return 1;
    }
    if(is_special_key(c)) {
        if(c == 'e' || c == 'E') {
            food_menu();
            return 2;
        }
        if(c == 'i' || c == 'I') {
            weopoan_menu();
            return 2;
        }
        if(c == 'p' || c == 'P') {
            spell_menu();
            return 2;
        }
        if(c == 'm' || c == 'M') {
            map_menu();
            return 2;
        }
        if(c == 's' || c == 'S') {
            for(int i = user.pos.y - 1; i <= user.pos.y + 1; i++) {
                for(int j = user.pos.x - 1; j <= user.pos.x + 1; j++) {
                    if(i < 0 || i >= LINES || j < 0 || j >= COLS)
                        continue;
                    update_visited(user.current_level, i, j);
                }
            }
            return 2;
        }
        if(c == '.' || c == '>') {
            if(user.levels[user.current_level].base[user.pos.y][user.pos.x] == '>') {
                if(user.current_level == 0) {
                    return -1;
                }
                user.current_level--;
                user.pos.y = user.levels[user.current_level].stair.y;
                user.pos.x = user.levels[user.current_level].stair.x;
                strcpy(messages_log[messages_count++], "You entered the previous level!");
            }
            else {
                return -1;
            }
            return 2;
        }
        if(c == ',' || c == '<') {
            if(user.levels[user.current_level].base[user.pos.y][user.pos.x] == '<') {
                if(user.current_level == 4) {
                    for(int i = 0; i < user.levels[4].rooms[0].enemies_count ; i++) {
                        if(user.levels[4].rooms[0].enemies[i].type != -1) {
                            strcpy(messages_log[messages_count++], "You have to kill all the enemies!");
                            return -1;
                        }
                    }
                    return 5;
                }
                user.current_level++;
                user.pos.y = user.levels[user.current_level].spawn.y;
                user.pos.x = user.levels[user.current_level].spawn.x;
                strcpy(messages_log[messages_count++], "You entered the next level!");
                update_visited(user.current_level, user.pos.y, user.pos.x);
            }
            else {
                return -1;
            }
            return 2;
        }
        if(c == '/' || c == '?') {
            if(user.levels[user.current_level].has_enchant_room && user.levels[user.current_level].secret_doors[0].y == user.pos.y && user.levels[user.current_level].secret_doors[0].x == user.pos.x) {
                user.pos.y = user.levels[user.current_level].secret_doors[1].y;
                user.pos.x = user.levels[user.current_level].secret_doors[1].x;
                strcpy(messages_log[messages_count++], "You passed the secret door!");
                update_visited(user.current_level, user.pos.y, user.pos.x);
            }
            else if (user.levels[user.current_level].has_enchant_room && user.levels[user.current_level].secret_doors[1].y == user.pos.y && user.levels[user.current_level].secret_doors[1].x == user.pos.x) {
                user.pos.y = user.levels[user.current_level].secret_doors[0].y;
                user.pos.x = user.levels[user.current_level].secret_doors[0].x;
                strcpy(messages_log[messages_count++], "You passed the secret door!");
                update_visited(user.current_level, user.pos.y, user.pos.x);
            }
            else {
                return -1;
            }
            return 2;
        }
    }
    if(c == 'f' || c == 'F') {
        return 3;
    }
    if(c == 'g' || c == 'G') {
        return 4;
    }
    if(c == 'q' || c == 'Q') {
        return 7;
    }
    return -1;
}

int pick_up_item() {
    int l = user.current_level, y = user.pos.y, x = user.pos.x;
    if(user.levels[l].items[y][x] != 0) {
        int item = user.levels[l].items[y][x];
        if(item == 2 || item == 3) {
            user.golds += 15 + (rand() % 10);
            strcpy(messages_log[messages_count++], "You picked up some golds!");
        }
        else if(item == 4 || item == 5) {
            user.golds += 50 + (rand() % 20);
            strcpy(messages_log[messages_count++], "You picked up some golds!");
        }
        else if(item >= 10 && item < 16) {
            int weapoan_type = item / 2 - 4;
            user.weapoans[weapoan_type].count += user.weapoans[weapoan_type].collect;
            strcpy(messages_log[messages_count++], "You picked up a weapoan!");
        }
        else if(item == 16 || item == 17) {
            user.weapoans[4].count = 1;
            strcpy(messages_log[messages_count++], "You picked up a sword!");
        }
        else if(item >= 20 && item < 26) {
            int spell_type = item / 2 - 10;
            user.spells[spell_type].count += 1;
            strcpy(messages_log[messages_count++], "You picked up a spell!");
        }
        else if(item >= 30 && item < 38) {
            if(user.foods_count == 5) {
                strcpy(messages_log[messages_count++], "You can't pick up more food!");
                return 0;
            }
            int food_type = item / 2 - 15;
            user.foods[user.foods_count].type = food_type;
            user.foods[user.foods_count++].collect_time = user.timer;
            strcpy(messages_log[messages_count++], "You picked up a food!");
        }
        user.levels[l].items[y][x] = 0;
        user.levels[l].items[y][x + (item % 2 == 0 ? 1 : -1)] = 0;
    }
    if(user.levels[l].trown[y][x].existence == 1) {
        for(int i = 0; i < 3; i++) {
            user.weapoans[i+1].count += user.levels[l].trown[y][x].count[i];
            user.levels[l].trown[y][x].count[i] = 0;
        }
        user.levels[l].trown[y][x].existence = 0;
    }
}

int apply_attack() {
    int wep = user.selected_weapoan;
    int l = user.current_level, y = user.pos.y, x = user.pos.x;
    int r = user.levels[l].which_room[y][x];
    int damage = user.weapoans[wep].damage;
    int damage_spell_passed = user.timer - user.spells[2].last_time;
    if(damage_spell_passed < 10)
        damage *= 2;
    if(r == -1) 
        return -1;
    if(wep == 0 || wep == 4) {
        for(int i = y-1; i <= y+1; i++) {
            for(int j = x-1; j <= x+1; j++) {
                if(user.levels[l].enemies[i][j] != 0) {
                    for(int k = 0; k < user.levels[l].rooms[r].enemies_count; k++) {
                        if(user.levels[l].rooms[r].enemies[k].type != -1 && user.levels[l].rooms[r].enemies[k].pos.y == i && user.levels[l].rooms[r].enemies[k].pos.x == j) {
                            user.levels[l].rooms[r].enemies[k].health -= damage;
                            user.levels[l].rooms[r].enemies[k].last_seen = user.timer;
                            if(user.levels[l].rooms[r].enemies[k].health <= 0) {
                                char message[150];
                                sprintf(message, "You killed a %s with your %s!", enemies_names[user.levels[l].rooms[r].enemies[k].type], weapoan_names[wep]);
                                strcpy(messages_log[messages_count++], message);
                                user.levels[l].rooms[r].enemies[k].type = -1;
                                user.levels[l].enemies[i][j] = 0;
                            }
                            else {
                                char message[150];
                                sprintf(message, "You hit a %s with your %s! (%d)", enemies_names[user.levels[l].rooms[r].enemies[k].type], weapoan_names[wep], damage);
                                strcpy(messages_log[messages_count++], message);
                            }
                        }
                    }
                }
            }
        }
    }
    else {
        mvprintw(1, 4, "Choose a direction to attack:");
        refresh();
        int c = getch();
        if(!is_move_key(c))
            return -1;
        int dir;
        if(c == 'y' || c == 'Y')
            dir = 0;
        else if(c == 'j' || c == 'J')
            dir = 1;
        else if(c == 'u' || c == 'U')
            dir = 2;
        else if(c == 'h' || c == 'H')
            dir = 3;
        else if(c == 'l' || c == 'L')
            dir = 5;
        else if(c == 'b' || c == 'B')
            dir = 6;
        else if(c == 'k' || c == 'K')
            dir = 7;
        else if(c == 'n' || c == 'N')
            dir = 8;
        int dy = (dir / 3) - 1;
        int dx = (dir % 3) - 1;
        int yp = y, xp = x, fail = 1;
        for(int i = 0; i < user.weapoans[wep].range; i++) {
            if(user.levels[l].base[yp + dy][xp + dx] != '.' || user.levels[l].items[yp + dy][xp + dx] != 0) {
                break;
            }
            yp += dy;
            xp += dx;
            if(user.levels[l].rooms[user.levels[l].which_room[yp][xp]].type == 1) {
                attron(COLOR_PAIR(103));
            }
            if(user.levels[l].rooms[user.levels[l].which_room[yp][xp]].type == 2) {
                attron(COLOR_PAIR(109));
            }
            mvprintw(yp, xp, "\u27B6");
            if(yp - dy != user.pos.y || xp - dx != user.pos.x) {
                mvprintw(yp - dy, xp - dx, ".");
            }
            if(user.levels[l].rooms[user.levels[l].which_room[yp][xp]].type == 1) {
                attroff(COLOR_PAIR(103));
            }
            if(user.levels[l].rooms[user.levels[l].which_room[yp][xp]].type == 2) {
                attroff(COLOR_PAIR(109));
            }
            refresh();
            if(user.levels[l].enemies[yp][xp] != 0) {
                fail = 0;
                for(int k = 0; k < user.levels[l].rooms[r].enemies_count; k++) {
                    if(user.levels[l].rooms[r].enemies[k].type != -1 && user.levels[l].rooms[r].enemies[k].pos.y == yp && user.levels[l].rooms[r].enemies[k].pos.x == xp) {
                        user.levels[l].rooms[r].enemies[k].health -= damage;
                        user.levels[l].rooms[r].enemies[k].last_seen = user.timer;
                        if(user.levels[l].rooms[r].enemies[k].health <= 0) {
                            char message[150];
                            sprintf(message, "You killed a %s with your %s!", enemies_names[user.levels[l].rooms[r].enemies[k].type], weapoan_names[wep]);
                            strcpy(messages_log[messages_count++], message);
                            user.levels[l].rooms[r].enemies[k].type = -1;
                            user.levels[l].enemies[yp][xp] = 0;
                        }
                        else {
                            char message[150];
                            sprintf(message, "You hit a %s with your %s! (%d)", enemies_names[user.levels[l].rooms[r].enemies[k].type], weapoan_names[wep], damage);
                            strcpy(messages_log[messages_count++], message);
                        }
                    }
                }
                break;
            }
            napms(50);
        }
        if(fail) {
            char message[150];
            sprintf(message, "You missed a shot with your %s!", weapoan_names[wep]);
            strcpy(messages_log[messages_count++], message);
            user.levels[l].trown[yp][xp].count[wep - 1] += 1;
            user.levels[l].trown[yp][xp].existence = 1;
        }
        user.weapoans[wep].count -= 1;
        if(user.weapoans[wep].count == 0) {
            strcat(messages_log[messages_count++], "Your out of ammo!");
            user.selected_weapoan = 0;
        }
    }
}

int move_enemies() {
    int l = user.current_level, y = user.pos.y, x = user.pos.x;
    int r = user.levels[l].which_room[y][x];
    if(r == -1)
        return 1;
    for(int i = 0; i < user.levels[l].rooms[r].enemies_count; i++) { 
        if(user.levels[l].rooms[r].enemies[i].type != -1) {
            if((user.levels[l].rooms[r].enemies[i].type == 2 || user.levels[l].rooms[r].enemies[i].type == 4) && user.timer - user.levels[l].rooms[r].enemies[i].last_seen >= 5) {
                continue;
            }
            int yn = user.levels[l].rooms[r].enemies[i].pos.y, xn = user.levels[l].rooms[r].enemies[i].pos.x;
            if(abs(yn - y) <= 1 && abs(xn - x) <= 1) {
                int attack_damage = user.levels[l].rooms[r].enemies[i].damage + (2 * user.difficulty) + (1 - rand() % 3);
                user.health -= attack_damage;
                user.levels[l].rooms[r].enemies[i].last_seen = user.timer;
                char message[150];
                snprintf(message, sizeof(message), "A %s attacked you! (%d)", enemies_names[user.levels[l].rooms[r].enemies[i].type], attack_damage);
                strcpy(messages_log[messages_count++], message);
            }
            else {
                POSITION next;
                int min_dis = 100000;
                for(int yp = yn - 1; yp <= yn + 1; yp++) {
                    for(int xp = xn - 1; xp <= xn + 1; xp++) {
                        if(user.levels[l].base[yp][xp] == '.' && user.levels[l].items[yp][xp] == 0
                        && user.levels[l].enemies[yp][xp] == 0 && user.levels[l].trown[yp][xp].existence == 0
                        && (yp != y || xp != x)) {
                            int dis = abs(yp - y) + abs(xp - x);
                            if(dis < min_dis || (dis == min_dis && (rand() % 2 == 0))) {
                                next.y = yp;
                                next.x = xp;
                                min_dis = dis;
                            }
                        }

                    }
                }
                if(min_dis == 100000) {
                    return 0;
                }
                user.levels[l].enemies[yn][xn] = 0;
                user.levels[l].enemies[next.y][next.x] = user.levels[l].rooms[r].enemies[i].symbol;
                user.levels[l].rooms[r].enemies[i].pos.y = next.y;
                user.levels[l].rooms[r].enemies[i].pos.x = next.x;
            }
            

        }
    }
}

int update_visited(int l, int y, int x) {
    if(user.levels[l].visited[y][x] == 0) {
        if(user.levels[l].has_enchant_room && user.levels[l].secret_doors[0].y == y && user.levels[l].secret_doors[0].x == x) {
            strcpy(messages_log[messages_count++], "You found the secret door!");
        }
    }
    user.levels[l].visited[y][x] = 1;

    if(user.levels[l].which_room[y][x] != -1) {
        if(user.levels[l].rooms[user.levels[l].which_room[y][x]].visited == 0) {
            if(user.levels[l].rooms[user.levels[l].which_room[y][x]].type == 1) {
                strcpy(messages_log[messages_count++], "You found the secret room!");
            }
            strcpy(messages_log[messages_count++], "You found a new room!");
        }
        user.levels[l].rooms[user.levels[l].which_room[y][x]].visited = 1;
    }
}

int update_hunger() {
    if(user.timer - user.last_meal >= 25)
        user.hunger -= 1;
    if(user.hunger < 0)
        user.hunger = 0;
}

int update_health() {
    if(user.hunger == 0)
        user.health -= 2;
    if(user.hunger == default_hunger)
        user.health += 2;
    if(user.levels[user.current_level].which_room[user.pos.y][user.pos.x] != -1 && user.levels[user.current_level].rooms[user.levels[user.current_level].which_room[user.pos.y][user.pos.x]].type == 1) {
        user.last_attack = user.timer;
        user.health -= 1;
    }
    if(user.timer - user.spells[0].last_time < 10)
        user.health += 3;
    if(user.timer - user.last_attack > 30)
        user.health += 1;
    if(user.levels[user.current_level].base[user.pos.y][user.pos.x] == '^') {
        user.health -= (5 + 3 * user.difficulty);
        strcpy(messages_log[messages_count++], "You stepped on a trap!");
    }

    if(user.health > default_health[user.difficulty])
        user.health = default_health[user.difficulty];
}

int update_food() {
    for(int i = 0; i < user.foods_count; i++) {
        if(user.timer - user.foods[i].collect_time >= 40) {
            if(user.foods[i].type == 0) {
                user.foods[i].type = 3;
                user.foods[i].collect_time = user.timer;
            }
            else if(user.foods[i].type == 1 || user.foods[i].type == 2) {
                user.foods[i].type = 0;
                user.foods[i].collect_time = user.timer;
            }
        }
    }
}

int update_music(int room_type) {
    int music_num;
    if(room_type == 0)
        music_num = user.music_stat_normal;
    else if(room_type == 1)
        music_num = 3;
    else if(room_type == 2)
        music_num = 4;
    else if(room_type == 3)
        music_num = 5;
    if(user.music_stat != music_num) { 
        char path_prev[M];
        snprintf(path_prev, sizeof(path_prev), "music%d.mp3", user.music_stat);
        Mix_Music *music = Mix_LoadMUS(path_prev);
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        if(music_num != 0) {
            SDL_Init(SDL_INIT_AUDIO);
            Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
            char path[M];
            snprintf(path, sizeof(path), "music%d.mp3", music_num);
            Mix_Music *music = Mix_LoadMUS(path);
            Mix_PlayMusic(music, -1);
        }
        user.music_stat = music_num;
    }
}

int food_menu() {
    int selected = 0;
    while(1) {
        clear();
        attron(COLOR_PAIR(106));
        draw_menu_border();
        attroff(COLOR_PAIR(106));
        attron(A_BOLD | A_BLINK | COLOR_PAIR(108));
        mvprintw(6, COLS/2 - 5, "FOODs MENU");
        attroff(A_BOLD | A_BLINK | COLOR_PAIR(108));
        attron(A_DIM);
        mvprintw(7, COLS/2 - 19, "(Press ENTER to Select and ESC to Exit)");
        attroff(A_DIM);
        for(int i = 0; i < 5; i++) {
            if(user.foods[i].type == -1) {
                attron(A_DIM);
            }
            if(i == selected) {
                attron(A_REVERSE);
                mvprintw(12 + 2*i, COLS/2 - 22, "                                             ");
            }
            if(user.foods[i].type == -1) {
                mvprintw(12 + 2*i, COLS/2 - 22, "[Empty Slot]");
            }
            else if(user.foods[i].type == 3) {
                mvprintw(12 + 2*i, COLS/2 - 22, "%s", food_names[0]);
                mvprintw(12 + 2*i, COLS/2 + 14, "(slot %d)", i+1);
            }
            else {
                mvprintw(12 + 2*i, COLS/2 - 22, "%s", food_names[user.foods[i].type]);
                mvprintw(12 + 2*i, COLS/2 + 14, "(slot %d)", i+1);
            }
            if(i == selected) {
                attroff(A_REVERSE);
            }
            if(user.foods[i].type == -1) {
                attroff(A_DIM);
            }
        }
        refresh();
        int c = getch();
        if(c == KEY_UP) {
            if(selected == 0)
                selected = user.foods_count - 1;
            else
                selected--;
        }
        if(c == KEY_DOWN) {
            if(selected == user.foods_count - 1)
                selected = 0;
            else
                selected++;
        }
        if(c == KEY_ENT) {
            if(user.foods[selected].type == 3) {
                user.health -= 10;
                user.hunger += 30;
                if(user.hunger > default_hunger)
                    user.hunger = default_hunger;
                strcpy(messages_log[messages_count++], "Sorry, Your food was corrupted!");
            }
            else {
                user.health += 10;
                user.hunger += 40;
                if(user.health > default_health[user.difficulty])
                    user.health = default_health[user.difficulty];
                if(user.hunger > default_hunger)
                    user.hunger = default_hunger;
                if(user.foods[selected].type == 1)
                    user.spells[2].last_time = user.timer;
                else if(user.foods[selected].type == 2)
                    user.spells[1].last_time = user.timer;
                strcpy(messages_log[messages_count++], "Food consumed successfully!");
            }
            if(user.health > default_health[user.difficulty])
                user.health = default_health[user.difficulty];
            user.foods[selected].type = user.foods[user.foods_count - 1].type;
            user.foods[selected].collect_time = user.foods[user.foods_count - 1].collect_time;
            user.foods[user.foods_count - 1].type = -1;
            user.foods_count--;
            user.last_meal = user.timer;
            break;
        }
        if(c == KEY_ESC) {
            break;
        }
    }
}

int weopoan_menu() {
    int selected = 0;
    if(user.selected_weapoan == 4)
        selected = 1;
    else if(user.selected_weapoan > 0)
        selected = user.selected_weapoan + 1;
    int number[5] = {0, 4, 1, 2, 3};
    int line[5] = {12, 14, 19, 21, 23};
    while(1) {
        clear();
        attron(COLOR_PAIR(106));
        draw_menu_border();
        attroff(COLOR_PAIR(106));
        attron(A_BOLD | A_BLINK | COLOR_PAIR(108));
        mvprintw(6, COLS/2 - 6, "WEAPOANs MENU");
        attroff(A_BOLD | A_BLINK | COLOR_PAIR(108));
        attron(A_DIM);
        mvprintw(7, COLS/2 - 19, "(Press ENTER to Select and ESC to Exit)");
        attroff(A_DIM);
        attron(COLOR_PAIR(106));
        mvprintw(10, COLS/2 - 22, "Melee Weapoans:");
        mvprintw(17, COLS/2 - 22, "Ranged Weapoans:");
        attroff(COLOR_PAIR(106));
        for(int i = 0; i < 5; i++) {
            if(user.weapoans[number[i]].count == 0) {
                attron(A_DIM);
            }
            if(i == selected) {
                attron(A_REVERSE);
                mvprintw(line[i], COLS/2 - 22, "                                             ");
            }
            mvprintw(line[i], COLS/2 - 22, "%s", weapoan_names[number[i]]);
            mvprintw(line[i], COLS/2 + 21, "X%d", user.weapoans[number[i]].count);
            if(i == selected) {
                attroff(A_REVERSE);
            }
            if(user.weapoans[number[i]].count == 0) {
                attroff(A_DIM);
            }
        }
        refresh();
        int c = getch();
        if(c == KEY_UP) {
            if(selected == 0)
                selected = 4;
            else
                selected--;
        }
        if(c == KEY_DOWN) {
            if(selected == 4)
                selected = 0;
            else
                selected++;
        }
        if(c == KEY_ENT) {
            if(user.weapoans[number[selected]].count     != 0) {
                user.selected_weapoan = number[selected];
                strcpy(messages_log[messages_count++], "Weapoan changed successfully!");
            }
            else {
                strcpy(messages_log[messages_count++], "Sorry, You don't own that weapoan!");
            }
            break;
        }
        if(c == KEY_ESC) {
            break;
        }
    }

}

int spell_menu() {
    int selected = 0;
    while(1) {
        clear();
        attron(COLOR_PAIR(106));
        draw_menu_border();
        attroff(COLOR_PAIR(106));
        attron(A_BOLD | A_BLINK | COLOR_PAIR(108));
        mvprintw(6, COLS/2 - 5, "SPELLs MENU");
        attroff(A_BOLD | A_BLINK | COLOR_PAIR(108));
        attron(A_DIM);
        mvprintw(7, COLS/2 - 19, "(Press ENTER to Select and ESC to Exit)");
        attroff(A_DIM);
        for(int i = 0; i < 3; i++) {
            if(user.spells[i].count == 0) {
                attron(A_DIM);
            }
            if(i == selected) {
                attron(A_REVERSE);
                mvprintw(12 + 2*i, COLS/2 - 22, "                                             ");
            }
            mvprintw(12 + 2*i, COLS/2 - 22, "%s", spell_names[i]);
            mvprintw(12 + 2*i, COLS/2 + 21, "X%d", user.spells[i].count);
            if(i == selected) {
                attroff(A_REVERSE);
            }
            if(user.spells[i].count == 0) {
                attroff(A_DIM);
            }
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
            if(user.spells[selected].count != 0) {
                user.spells[selected].last_time = user.timer;
                user.spells[selected].count--;
                strcpy(messages_log[messages_count++], "Spell used successfully!");
            }
            else {
                strcpy(messages_log[messages_count++], "Sorry, You don't own that spell!");
            }
            break;
        }
        if(c == KEY_ESC) {
            break;
        }
    }
}

int map_menu() {
    clear();
    int l = user.current_level;
    attron(A_DIM);
    for(int i = 0; i < LINES; i++) {
        for(int j = 0; j < COLS; j++) {
            int base = user.levels[l].base[i][j];
            if(base == '^') {
                mvprintw(i, j, ".");
            }
            else if(base == '?') {
                if(user.levels[l].base[i][j+1] == '-' || user.levels[l].base[i][j+1] == '+') {
                    mvprintw(i, j, "-");
                }
                else{
                    mvprintw(i, j, "|");
                }
            }
            else
                mvprintw(i, j, "%c", base);
        }
    }
    attroff(A_DIM);

    attron(A_BLINK | A_UNDERLINE);
    mvprintw(1, COLS/2 - 16, "(Press any key to EXIT the map)");
    attroff(A_BLINK | A_UNDERLINE);

    attron(COLOR_PAIR(106));
    draw_menu_border();
    attroff(COLOR_PAIR(106));


    refresh();
    getch();
}