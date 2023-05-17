#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 20
#define MAX_USERS 50
#define MAX_FRIENDS 50

typedef struct{
    char user;
    int age;
    char mail[MAX_LENGTH];
    char loc[MAX_LENGTH];
    char hobby[5];
    int friends[MAX_FRIENDS];
} User;
typedef struct {
    int num;
    User users[MAX_USERS];
} List;

List readUsersFromFile(const char* filename) {
    List list;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        list.num = 0;
        return list;
    }

    char line[MAX_LENGTH];
    char* token;

    list.num = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        User user;

        token = strtok(line, ",");
        if (token == NULL) {
            printf("Invalid line format: %s\n", line);
            continue;
        }
        strncpy(&user.user, token, sizeof(user.user));

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Invalid line format: %s\n", line);
            continue;
        }
        user.age = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Invalid line format: %s\n", line);
            continue;
        }
        strncpy(user.mail, token, sizeof(user.mail));
        //loc
        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("Invalid line format: %s\n", line);
            continue;
        }
        strncpy(user.loc, token, sizeof(user.loc));

        //hobby
        token = strtok(NULL, "(");
        if (token == NULL) {
            printf("Invalid line format: %s\n", line);
            continue;
        }

        token = strtok(NULL, ",");
        for (int i = 0; i < 5 && token != NULL; i++) {
            while (*token == ' ')
                token++;
            int len = strlen(token);
            while (len > 0 && token[len - 1] == ' ')
                len--;
            strncpy(&user.hobby[i], token, len);
            token = strtok(NULL, ",");
        }

        //final hobby
        token = strtok(NULL, ")");
        if (token == NULL) {
            printf("Invalid line format: %s\n", line);
            continue;
        }

        //usuari afegit a la llista
        list.users[list.num++] = user;
    }

    fclose(file);
    return list;
}

void print_menu(){
    printf("1.Insertar usuari nou\n2.Mostrar usuaris existents\n3.Seleccionar usuari\n4.Sortir\n");
}
int menu(){
    List list = readUsersFromFile(".../Practica EDA2/users.txt");

    int opt = -1;
    while (opt == -1){
        print_menu();
        scanf("%d", &opt);
        if (opt == 1){
            //Insertar usuari nou
            int newuser = -1;
            int i = 0;
            printf("Usuari:");
            scanf("%s", &list.users[list.num].user);
            printf("\nEdat:");
            scanf("%d",  &list.users[list.num].age);
            printf("\nMail:");
            scanf(" %[^\n]",  &list.users[list.num].mail);
            printf("\nUbicacio:");
            scanf(" %[^\n]",  &list.users[list.num].loc);
            printf("\n5 Hobbies:");
            for (int j = 0; j < 5; j++) {
                scanf("%s",  &list.users[list.num].hobby[j]);
            }
            for (int j = 0; j < MAX_FRIENDS; j++) {
                list.users[list.num].friends[j] = -1;
            }
            list.num++;
        }
        else if (opt == 2){
            for (int i = 0; i < list.num; i++){
                if ( &list.users[i].user != -1){
                    printf("%s\tEdat:%d\tHobbies:%s", &list.users[i].user, &list.users[i].age, &list.users[i].hobby);
                }
            }
            return 0;
        }
        else if (opt == 3){
            //Selecció usuari
            char selected_user[MAX_LENGTH];
            printf("Introdueix el nom de l'usuari seleccionat:");
            scanf("%s", &selected_user);
            int found_user = -1;
            for (int i = 0; i <= list.num; i++){
                if ( &list.users[i].user != -1){
                    if (strcmp( &list.users[i].user,selected_user) == 0){
                        found_user=i;
                        break;
                    }
                }
            }
            if (found_user == -1){
                printf("No s'ha trobat l'usuari seleccionat\n");
            }
            else{
                printf("Nom:%s\n", &list.users[found_user].user);
                printf("Edat:%d\n", list.users[found_user].age);
                printf("Mail:%s\n", list.users[found_user].mail);
                printf("Ubicacio:%s\n", list.users[found_user].loc);
                printf("Hobbies:%s,%s,%s,%s,%s\n", &list.users[found_user].hobby[0], &list.users[found_user].hobby[1], &list.users[found_user].hobby[2], &list.users[found_user].hobby[3], &list.users[found_user].hobby[4]);
                int selected_user = found_user;
                //Processos usuari
                printf("\n1. Solicitud amistat\n2. Canviar d'usuari");
                opt = -1;
                while (opt == -1) {
                    if (opt == 1) {
                        char friend[MAX_LENGTH];
                        scanf("%s", &friend);
                        int found_user = -1;
                        for (int i = 0; i < list.num; i++) {
                            if (strcmp(&list.users[i].user, friend) == 0) {
                                int friend = i;
                                found_user = 1;
                                break;
                            }
                        }
                        if (found_user == 1){
                            int newfriend = 0;
                            int i = 0;
                            while ((newfriend == 0) && (i <= MAX_FRIENDS)){
                                if (list.users[selected_user].friends[i] == -1 ){
                                    //Solicitud amistat
                                }
                                i += 1;

                            }
                        }
                        else printf("No s'ha trobat l'usuari.");
                    }
                }
            }
        }
        else if (opt == 4) {
            //Tancar
            return -1;
        }
        else {
            printf("Introdueix un valor de 1 a 4\n");
        }

    }
    return 0;
}

int main() {
    int stop = 0;
    while (stop==0) {
        stop = menu();
    }
    return 0;
}
// User* userlist = malloc(MAX_USERS*sizeof(User));
//    for (int i = 0; i < MAX_USERS; i++){
//        userlist[i].user = -1;