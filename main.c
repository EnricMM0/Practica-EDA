
#include "main.h"




void print_menu(){
    printf("\n1.Insertar usuari nou\n2.Mostrar usuaris existents\n3.Seleccionar usuari\n4.Imprimir paraules mes fetes servir\n5.Sortir\n");
}

void menu(User **head, Dict dict){
    int opt = -1;
    while (opt == -1){
        print_menu();
        fflush(stdin); //buidem input consola
        scanf("%d", &opt);
        if (opt == 1){
            //Insertar usuari nou
            usuari_nou(head);
            opt = -1;
        }
        else if (opt == 2){
            User *index = *head; // creem usuari index que recorrerà la llista 'linkejada' de users
            if (index == NULL) {
                printf("No hi ha users.\n");
            }
            while (index != NULL) {
                printf("%s\tEdat:%d\tHobbies:%s,%s,%s,%s,%s\n", index->user, index->age, index->hobby[0],index->hobby[1],index->hobby[2],index->hobby[3],index->hobby[4]);
                index = index->next; //movem al següent user
            }
            printf("\n");
            opt = -1;
        }
        else if (opt == 3) {
            //Selecció usuari
            char selected_user[MAX_LENGTH];
            printf("Introdueix el nom de l'usuari seleccionat:");
            scanf("%s", selected_user);
            User *selected= trobar_user(*head, selected_user); //busquem usuari amb el username = selected_user
            if (selected != NULL) {
                printf("Nom:%s\n", selected->user);
                printf("Edat:%d\n", selected->age);
                printf("Mail:%s\n", selected->mail);
                printf("Ubicacio:%s\n", selected->loc);
                printf("Hobbies:%s,%s,%s,%s,%s\n", selected->hobby[0], selected->hobby[1], selected->hobby[2],selected->hobby[3], selected->hobby[4]);
                Friend *friend = selected->friends.head; // punter de amics que recorrerà llista
                printf("Amics:");
                while (friend != NULL) {
                    User* amic = trobar_user(*head, friend->user); //agafem l'user que tingui el nom que hi ha a la llista de strings de friends
                    printf("%s  ",amic->user);
                    friend = friend->next; // movem al següent amic
                }
                printf("\n");
            }
            //Processos usuari
            useractions(dict, selected, head); // submenú de accions usuari
            opt = -1;

        }
        else if (opt == 4){
            dict = readDictFromFile("../dict.txt");
            Dictsort(&dict); //ordenem diccionari
            printf("\nParaules mes usades:\n");
            printf("Ranking\tUsos\tParaula\n");
            for (int i = 0; i < 10; ++i) { //imprimim cada valor i clau en el diccionari de posicions 0 a 9
                printf("%d)\t%d\t%s\n", i+1, dict.value[i],dict.key[i]);
            }
            opt = -1;
        }
        else if (opt == 5) {
            //Tancar
            return;
        }
        else {
            printf("Introdueix un valor de 1 a 5\n");
            opt = -1;
        }
    }
}


int main() {
    User *head = NULL; //punter de usuari comença null, serà punter de primer usuari, establert en readUsersFromFile
    readUsersFromFile("../users.txt", &head); //carreguem informació de fitxers de text
    readPostsFromFile("../posts.txt", &head);
    readFriendsFromFile("../friends.txt",&head);
    Dict dict = readDictFromFile("../dict.txt");
    menu(&head,dict); //bucle principal de el programa
    return 0;
}