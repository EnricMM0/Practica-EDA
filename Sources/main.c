#include <stdio.h>
#include <stdlib.h>
#define MAX_LENGTH 20
#define MAX_USERS 50
#define MAX_FRIENDS 50
typedef struct{
    char user;
    int age;
    char mail;
    char loc;
    char hobby[5];
    char friends[MAX_FRIENDS];
    char closefriends[MAX_FRIENDS];
} User;

void print_menu(){
    printf("1.Insertar usuari nou\n2.Mostrar usuaris existents\n3.Seleccionar usuari\n4.Sortir\n");
}
void init_user(User* userlist){

}
int menu(){
    User* userlist = malloc(MAX_USERS*sizeof(User*));
    for (int i = 0; i < MAX_USERS; i++){
        userlist[i].user = -1;
    }
    int opt = -1;
    while (opt == -1){
        print_menu();
        scanf("%d", &opt);
        if (opt == 1){
            int newuser = -1;
            int i = 0;
            while ((newuser == -1) && (i <= 50)){
                User user;
                if (userlist[i].user == -1){
                    printf("Usuari:");
                    scanf("%s", &userlist[i].user);
                    printf("\nEdat:");
                    scanf("%d", &userlist[i].age);
                    printf("\nMail:");
                    scanf("%s", &userlist[i].mail);
                    printf("\nUbicacio:");
                    scanf("%s", &userlist[i].loc);
                    printf("\n5 Hobbies, separats per commes i espais:");
                    scanf("%s, %s, %s, %s, %s", &userlist[i].hobby[0],&userlist[i].hobby[1],&userlist[i].hobby[2],&userlist[i].hobby[3],&userlist[i].hobby[4]);
                    newuser = 0;
                }
                i += 1;
            }
            return 0;
        }
        else if (opt == 2){
            //Llista usuaris
            return 0;
        }
        else if (opt == 3){
            //Selecció usuari
            return 0;
        }
        else if (opt == 4) {
            //Tancar
            return -1;
        }
        else {
            printf("Introdueix un valor de 1 a 4\n");
            opt = -1;
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
