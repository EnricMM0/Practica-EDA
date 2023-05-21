#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 20
#define MAX_USERS 50
#define MAX_FRIENDS 50

typedef struct{
    char user[MAX_LENGTH];
    int age;
    char mail[MAX_LENGTH];
    char loc[MAX_LENGTH];
    char hobby[5][MAX_LENGTH];
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
        printf("No s'ha pogut obrir el fitxer %s\n", filename);
        list.num = 0;
        return list;
    }
    char line[200];

    list.num = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        User user;
        sscanf(line,"%[^,],%d,%[^,],%[^,],(%[^,],%[^,],%[^,],%[^,],%[^)])",user.user, &user.age, user.mail, user.loc,user.hobby[0],user.hobby[1],user.hobby[2],user.hobby[3],user.hobby[4] );
        list.users[list.num] = user;
        list.num++;
    }

    fclose(file);
    return list;
}

void print_menu(){
    printf("\n1.Insertar usuari nou\n2.Mostrar usuaris existents\n3.Seleccionar usuari\n4.Sortir\n");
}
int menu(List list){
    int opt = -1;
    while (opt == -1){
        print_menu();
        scanf("%d", &opt);
        if (opt == 1){
            //Insertar usuari nou
            User newuser;
            printf("Usuari:");
            scanf("%s", newuser.user);
            printf("\nEdat:");
            scanf("%d",  &newuser.age);
            printf("\nMail:");
            scanf(" %[^\n]",  newuser.mail);
            printf("\nUbicacio:");
            scanf(" %[^\n]",  newuser.loc);
            printf("\n5 Hobbies:");
            for (int j = 0; j < 5; j++) {
                scanf("%s",  newuser.hobby[j]);
            }
            for (int j = 0; j < MAX_FRIENDS; j++) {
                newuser.friends[j] = -1;
            }
            FILE* file = fopen("../users.txt","a");
            if (file == NULL) {
                printf("No s'ha pogut obrir el fitxer");
            }
            else{
                fprintf(file, "\n%s,%d,%s,%s,(%s,%s,%s,%s,%s)",newuser.user,newuser.age,newuser.mail,newuser.loc,newuser.hobby[0],newuser.hobby[1],newuser.hobby[2],newuser.hobby[3],newuser.hobby[4]);
                fclose(file);
                list = readUsersFromFile("../users.txt");
            }

        }
        else if (opt == 2){
            for (int i = 0; i < list.num; i++){
                printf("\n%s\tEdat:%d\tHobbies:%s,%s,%s,%s,%s", list.users[i].user, list.users[i].age, list.users[i].hobby[0],list.users[i].hobby[1],list.users[i].hobby[2],list.users[i].hobby[3],list.users[i].hobby[4]);
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
                printf("\n1. Solicitud amistat\n2. Canviar d'usuari\n");
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
    List list = readUsersFromFile("../users.txt");
    int stop = 0;
    while (stop==0) {
        stop = menu(list);
    }
    return 0;
}
// User* userlist = malloc(MAX_USERS*sizeof(User));
//    for (int i = 0; i < MAX_USERS; i++){
//        userlist[i].user = -1;