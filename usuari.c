#include "main.h"

User* trobar_user(User *head, char user[MAX_LENGTH]) {
    User *current = head;
    while (current != NULL) {
        if (strcmp(current->user, user) == 0) {
            return current;
        }
        current = current->next;
    }
    printf("No s'ha trobat l'usuari.\n");
    return NULL;
}
void useractions(Dict dict,User* selected, User **head) {
    int opt = -1;
    while (opt == -1) {
        fflush(stdin);
        printf("\n1.Solicitud amistat\n2.Publicacio\n3.Veure publicacions teves\n4.Veure publicacions d'amics\n5.Veure solicituds pendents\n6.Enrere\n");
        scanf("%d", &opt);
        if (opt == 1) {
            char friend[MAX_LENGTH];
            //solicitud amic
            printf("Introdueix el usuari de l'amic:");
            scanf("%s", friend);
            fflush(stdin);
            User* recipient = trobar_user(*head,friend);
            Friend* amic = selected->friends.head;
            int found = 0;
            while (amic->next != NULL){
                if (strcmp(amic->user, friend)==0){
                    found = 1;
                    break;
                }
            }
            if (found == 1){
                printf("Ja ets amic d'aquesta persona.\n");
            }
            else {
                envia_solicitud(selected, recipient);
                printf("Solicitud amistat enviada.\n");
                opt = -1;
            }

        }
        else if (opt == 2) {
            if (selected->num_posts >= MAX_POSTS) {
                printf("Has arribat al límit de publicacions.\n");
            } else {
                Post post;
                time(&post.t);
                printf("Temps:%s", ctime(&post.t));
                fflush(stdin);
                scanf("%[^\n]", post.text);
                selected->posts[selected->num_posts] = post;
                selected->num_posts++;
                FILE *file = fopen("../posts.txt", "a");
                if (file == NULL) {
                    printf("No s'ha pogut obrir el fitxer.\n");
                }
                else {
                    fprintf(file, "%s,%s|%lld\n", selected->user,selected->posts[selected->num_posts-1].text, selected->posts[selected->num_posts-1].t);
                }
                fclose(file);

                dict = readDictFromFile("../dict.txt");
                dict =*( dictionarycheck(&dict, &post));
                Dictsort(&dict);
                file = fopen("../dict.txt", "w");
                if (file == NULL) {
                    printf("No s'ha pogut obrir el fitxer.\n");
                } else {
                    for (int i = 0; i < dict.num; ++i) {
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
                    printf("\n- - - - - - - - - - - - - -\n%s%s", ctime(&selected->posts[i].t),selected->posts[i].text);
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
            Friend* friend = selected->friends.head;
            while (friend != NULL) {
                printf("\nVols veure les publicacions de %s? \n1.Acceptar\n2.Denegar\n", friend->user);
                scanf("%d", &opt);
                fflush(stdin);
                if (opt == 1) {
                    User *amic = trobar_user(*head, friend->user);
                    if (amic->num_posts > 0) {
                        for (int i = 0; i < amic->num_posts; ++i) {
                            printf("\n- - - - - - - - - - - - - -\n%s%s", ctime(&amic->posts[i].t),
                                   amic->posts[i].text);
                        }
                        printf("\n- - - - - - - - - - - - - -");
                    } else {
                        printf("\nEncara no ha fet publicacions.");
                    }
                    friend = friend->next;
                } else if (opt == 2) {
                    friend = friend->next;
                }
            }
            opt=-1;
        }
        else if (opt==5){
            proces_solicitud(selected, *head);
            opt =-1;
        }
        else{
            return;
        }
    }
}
void readUsersFromFile(const char* filename, User **head) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("No s'ha pogut obrir el fitxer %s\n", filename);
        return;
    }
    char line[200];

    while (fgets(line, sizeof(line), file) != NULL) {
        User *user = (User *)malloc(sizeof(User));
        sscanf(line,"%[^,],%d,%[^,],%[^,],(%[^,],%[^,],%[^,],%[^,],%[^)])",user->user, &user->age, user->mail, user->loc,user->hobby[0],user->hobby[1],user->hobby[2],user->hobby[3],user->hobby[4] );
        user->next = NULL;

        if(*head == NULL){
            *head = user;
        } else{
            User *last = *head;
            while (last->next != NULL){
                last = last->next;
            }
            last->next = user;
        }
    }
    fclose(file);
}
void usuari_nou(User **head) {
    User *newuser = (User*)malloc(sizeof(User));
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
    FILE* file = fopen("../users.txt","a");
    if (file == NULL) {
        printf("No s'ha pogut obrir el fitxer");
    }
    else{
        fprintf(file, "\n%s,%d,%s,%s,(%s,%s,%s,%s,%s)",newuser->user,newuser->age,newuser->mail,newuser->loc,newuser->hobby[0],newuser->hobby[1],newuser->hobby[2],newuser->hobby[3],newuser->hobby[4]);
        fclose(file);
    }

    User *last = *head;
    while (last->next != NULL){
        last = last->next;
    }
    last->next = newuser;

}