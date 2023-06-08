#include "main.h"

User* trobar_user(User *head, char user[MAX_LENGTH]) {
    User *current = head;
    while (current != NULL) {
        if (strcmp(current->user, user) == 0) {
            return current;
        }
        current = current->next; //recorrem llista fins que trobem el usuari que tingui el mateix nom que el 'char user'
    }
    printf("No s'ha trobat l'usuari.\n");
    return NULL;
}
void useractions(Dict dict,User* selected, User **head) {
    int opt = -1;
    while (opt == -1) {
        fflush(stdin); //buidem input consola
        printf("\n1.Solicitud amistat\n2.Publicacio\n3.Veure publicacions teves\n4.Veure publicacions d'amics\n5.Veure solicituds pendents\n6.Enrere\n");
        scanf("%d", &opt);
        if (opt == 1) {
            //solicitud amic
            char friend[MAX_LENGTH];
            printf("Introdueix el usuari de l'amic:");
            scanf("%s", friend);
            fflush(stdin); //buidem input consola
            User* recipient = trobar_user(*head,friend); //trobem usuari per enviar solicitud
            //busquem usuari en llista amics perquè no s'envïi solicitud a una persona que ja sigui amic
            Friend* amic = selected->friends.head;
            int found = 0;
            while (amic->next != NULL){
                if (strcmp(amic->user, friend)==0){
                    found = 1;
                    break;
                }
                amic = amic->next;
            }
            if (found == 1){
                printf("Ja ets amic d'aquesta persona.\n");
            }
            else {
                envia_solicitud(selected, recipient); //enviem solicitud amistat
                printf("Solicitud amistat enviada.\n");
                opt = -1;
            }

        }
        else if (opt == 2) {
            if (selected->num_posts >= MAX_POSTS) {
                printf("Has arribat al límit de publicacions.\n");
            } else {
                Post post; //declarem post nou i demanem el text
                time(&post.t);
                printf("Temps:%s", ctime(&post.t));
                fflush(stdin); //buidem input consola
                scanf("%[^\n]", post.text);
                selected->posts[selected->num_posts] = post; //establim ultim post com a el nou
                selected->num_posts++; //incrementem número posts
                FILE *file = fopen("../posts.txt", "a");
                if (file == NULL) {
                    printf("No s'ha pogut obrir el fitxer.\n");
                }
                else { //escribim post nou a posts.txt
                    fprintf(file, "%s,%s|%lld\n", selected->user,selected->posts[selected->num_posts-1].text, selected->posts[selected->num_posts-1].t);
                }
                fclose(file);

                dict = readDictFromFile("../dict.txt");
                dict =*( dictionarycheck(&dict, &post)); //actualitzem usuari
                Dictsort(&dict); //ordenem dict amb bubblesort
                file = fopen("../dict.txt", "w");
                if (file == NULL) {
                    printf("No s'ha pogut obrir el fitxer.\n");
                } else {
                    for (int i = 0; i < dict.num; ++i) { //actualitzem fitxer diccionari
                        fprintf(file, "%s,%d\n", dict.key[i], dict.value[i]);
                    }
                    fclose(file);
                }
                printf("Publicacio completada.\n");
                opt =-1;
            }
        }
        else if (opt==3){
            if (selected->num_posts > 0) {
                for (int i = 0; i < selected->num_posts; ++i) {
                    printf("\n- - - - - - - - - - - - - -\n%s%s", ctime(&selected->posts[i].t),selected->posts[i].text); //impressió post
                }
                printf("\n- - - - - - - - - - - - - -");
            }
            else{
                printf("\nEncara no has fet publicacions.");
            }
            opt =-1;
        }
        else if (opt==4){
            //Veure publicacions de amics
            Friend* friend = selected->friends.head; //friend és punter del primer
            while (friend != NULL) {
                printf("\nVols veure les publicacions de %s? \n1.Acceptar\n2.Denegar\n", friend->user);
                scanf("%d", &opt);
                fflush(stdin); //buidem input consola
                if (opt == 1) {
                    User *amic = trobar_user(*head, friend->user); //agafem user de friend
                    if (amic->num_posts > 0) {
                        for (int i = 0; i < amic->num_posts; ++i) {
                            printf("\n- - - - - - - - - - - - - -\n%s%s", ctime(&amic->posts[i].t),amic->posts[i].text);//impresió post
                        }
                        printf("\n- - - - - - - - - - - - - -");
                    } else {
                        printf("\nEncara no ha fet publicacions.");
                    }
                    friend = friend->next; //següent amic
                } else if (opt == 2) {
                    friend = friend->next; //següent amic
                }
                else printf("Escriu 1 o 2");
            }
            opt=-1;
        }
        else if (opt==5){
            proces_solicitud(selected, *head); // processem totes solicituds
            opt =-1;
        }
        else{
            return;
        }
    }
}

void readUsersFromFile(const char* filename, User **head) {
    FILE* file = fopen(filename, "r"); //Obrim fitxer en mode lectura
    if (file == NULL) {
        printf("No s'ha pogut obrir el fitxer %s\n", filename);
        return;
    }
    char line[200]; // creem búfer per llegir línees
    while (fgets(line, sizeof(line), file) != NULL) { //llegim cada línea
        User *user = (User *)malloc(sizeof(User)); //creem user i guardem informació
        sscanf(line,"%[^,],%d,%[^,],%[^,],(%[^,],%[^,],%[^,],%[^,],%[^)])",user->user, &user->age, user->mail, user->loc,user->hobby[0],user->hobby[1],user->hobby[2],user->hobby[3],user->hobby[4] );
        user->next = NULL; //és l'últim user, pertant no apunta a cap altre user

        if(*head == NULL){
            *head = user; //establim head com a punter del primer user
        } else{
            User *last = *head;
            while (last->next != NULL){
                last = last->next; //anem al últim user
            }
            last->next = user; // el últim user apunta al mou usuari
        }
    }
    fclose(file);
}
void usuari_nou(User **head) {
    User *newuser = (User*)malloc(sizeof(User)); //creem usuari nou i demanem informació
    printf("Usuari:");
    scanf("%s", newuser->user);
    printf("\nEdat:");
    scanf("%d", &newuser->age);
    printf("\nMail:");
    scanf(" %[^\n]", newuser->mail);
    printf("\nUbicacio:");
    scanf(" %[^\n]", newuser->loc);
    printf("\n5 Hobbies:");
    for (int j = 0; j < 5; j++) {
        scanf("%s", newuser->hobby[j]);
    }
    FILE* file = fopen("../users.txt","a"); //Obrim fitxer en mode append
    if (file == NULL) {
        printf("No s'ha pogut obrir el fitxer");
    }
    else{ //escribim el usuari nou al fitxer de text
        fprintf(file, "\n%s,%d,%s,%s,(%s,%s,%s,%s,%s)",newuser->user,newuser->age,newuser->mail,newuser->loc,newuser->hobby[0],newuser->hobby[1],newuser->hobby[2],newuser->hobby[3],newuser->hobby[4]);
        fclose(file);
    }

    User *last = *head;
    while (last->next != NULL){
        last = last->next; //anem al últim user
    }
    last->next = newuser; //usuari nou és el next del últim

}