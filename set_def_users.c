#include "head.h"

USER user;
USER_S user_s;

int set_random_user(char* username) {
    strcpy(user_s.username, username);
    user_s.all_saved_golds = (rand() % 2000) + 1;
    user_s.points = (rand() % 5000) + 1;
    user_s.finished_games = rand() % 15 + 1;
    user_s.first_game_time = time(NULL) - (rand() % (3600 * 24 * 30));
    char path[2 * M] = "./users/";
    strcat(path, username);
    DIR *dir = opendir(path);
    if(dir) {
        closedir(dir);
        return 1;
    }
    mkdir(path, 0700);
    strcat(path, "/user.dat");
    FILE* user_file = fopen(path, "wb");
    fwrite(&user, sizeof(USER), 1, user_file);
    fclose(user_file);
    strcpy(path, "./users/");
    strcat(path, username);
    closedir(opendir(path));
    strcat(path, "/user_s.dat");
    FILE* user_s_file = fopen(path, "wb");
    fwrite(&user_s, sizeof(USER_S), 1, user_s_file);
    fclose(user_s_file);
}

int main() {
    srand(time(NULL));
    DIR* users = opendir("./users");
    if(users) {
        closedir(users);
    }
    else {
        mkdir("./users", 0700);
    }

    int n;
    char username[M];
    printf("Enter the number of random users you want to create (1-40): ");
    scanf("%d", &n);
    if(n < 1 || n > 40) {
        printf("Invalid input. Please enter a number between 1 and 40.\n");
        return 1;
    }
    for(int i = 0; i < n; i++) {
        printf("Enter username for user %d: ", i + 1);
        scanf("%s", username);
        if(set_random_user(username))
            printf("username already exists.\n");
        else
            printf("User %s has been created successfully.\n", username);
    }


}