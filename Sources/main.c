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
    char friends[MAX_FRIENDS];
    char closefriends[MAX_FRIENDS];
} User;

void print_menu(){
    printf("1.Insertar usuari nou\n2.Mostrar usuaris existents\n3.Seleccionar usuari\n4.Sortir\n");
}

int menu(){
    User* userlist = malloc(MAX_USERS*sizeof(User));
    for (int i = 0; i < MAX_USERS; i++){
        userlist[i].user = -1;
    }
    int opt = -1;
    while (opt == -1){
        print_menu();
        scanf("%d", &opt);
        if (opt == 1){
            //Insertar usuari nou
            int newuser = -1;
            int i = 0;
            while ((newuser == -1) && (i <= MAX_USERS)){
                if (userlist[i].user == -1){
                    printf("Usuari:");
                    scanf("%s", &userlist[i].user);
                    printf("\nEdat:");
                    scanf("%d", &userlist[i].age);
                    printf("\nMail:");
                    scanf(" %[^\n]", &userlist[i].mail);
                    printf("\nUbicacio:");
                    scanf(" %[^\n]", &userlist[i].loc);
                    printf("\n5 Hobbies:");
                    for (int j = 0; j < 5; j++) {
                        scanf("%s", &userlist[i].hobby[j]);
                    }
                    newuser = 0;
                }
                i += 1;
            }

        }
        else if (opt == 2){
            for (int i = 0; i < MAX_USERS; i++){
                if (userlist[i].user != -1){
                    printf("%s\tEdat:%d\tHobbies:%s",userlist[i].user,userlist[i].age,userlist[i].hobby);
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
            for (int i = 0; i < MAX_USERS; i++){
                if (userlist[i].user != -1){
                    if (strcmp(&userlist[i].user,selected_user) == 0){
                        found_user = i;
                        break;
                    }
                }
            }
            if (found_user == -1){
                printf("No s'ha trobat l'usuari seleccionat\n");
            }
            else{
                printf("Nom:%s\n",userlist[found_user].user);
                printf("Edat:%d\n",userlist[found_user].age);
                printf("Mail:%s\n",userlist[found_user].mail);
                printf("Ubicacio:%s\n",userlist[found_user].loc);
                printf("Hobbies:%s,%s,%s,%s,%s\n",userlist[found_user].hobby[0],userlist[found_user].hobby[1],userlist[found_user].hobby[2],userlist[found_user].hobby[3],userlist[found_user].hobby[4]);
            }
        }
        else if (opt == 4) {
            //Tancar
            return -1;
        }
        else {
            printf("Introdueix un valor de 1 a 4\n");
            return 0;
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
