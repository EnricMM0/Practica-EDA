#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_LENGTH 20
#define MAX_POSTS 100
#define MAX_LENGTH_POST 120
#define MAX_WORDS 100
typedef struct {
    char text[MAX_LENGTH_POST];
    time_t t;
} Post;

typedef struct solicitud{
    char sender[MAX_LENGTH];
    struct solicitud* next;
} Soli;

typedef struct  {
    Soli* front;
    Soli* rear;
} QueueSoli ;

typedef struct friend_t{
    char user[MAX_LENGTH];
    struct friend_t *next;
} Friend;

typedef struct {
    Friend *head;
}PilaFriends;

typedef struct user_t{
    char user[MAX_LENGTH];
    int age;
    char mail[MAX_LENGTH];
    char loc[MAX_LENGTH];
    char hobby[5][MAX_LENGTH];
    PilaFriends friends;
    Post posts[MAX_POSTS];
    int num_posts;
    QueueSoli sol;
    struct user_t *next;
} User;

typedef struct{
    char key[MAX_WORDS][MAX_LENGTH];
    int value[MAX_WORDS];
    int num;
} Dict;


void envia_solicitud(User* enviador, User* recipient){
    Soli* newSoli = (Soli*) malloc(sizeof(Soli));
    strcpy(newSoli->sender,enviador->user);
    newSoli->next = NULL;
    if (recipient->sol.front == NULL){
        recipient->sol.front = newSoli;
        recipient->sol.rear = newSoli;
    }
    else{
        recipient->sol.rear->next = newSoli;
        recipient->sol.rear= newSoli;
    }
}
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

void afegir_friend(User *user,char friend[MAX_LENGTH], User *head){
    //1ra direcció
    Friend *new_friend = (Friend *) malloc(sizeof(Friend));
    strcpy(new_friend->user,friend);
    new_friend->next = NULL;
    if (user->friends.head == NULL) {
        user->friends.head = new_friend;
    } else {
        Friend *index = user->friends.head;
        while (index->next != NULL) {
            index = index->next;
        }
        index->next = new_friend;
    }
    //2na direcció
    Friend *new_friend2 = (Friend *) malloc(sizeof(Friend));
    User* user2 = trobar_user(head, friend);
    strcpy(new_friend2->user, user->user);
    new_friend2->next = NULL;
    if (user2->friends.head == NULL) {
        user2->friends.head = new_friend2;
    } else {
        Friend *index2 = user2->friends.head;
        while (index2->next != NULL) {
            index2 = index2->next;
        }
        index2->next = new_friend2;
    }
}


void proces_solicitud(User *user,User* head) {
    Soli *index = user->sol.front;
    if (index==NULL) {
        printf("No tens solicituds pendents.\n");
        return;
    }
    //processar solicituds
    while(index != NULL){
        int opt = -1;
        printf("Tens solicitud de: %s\n", index->sender);
        printf("1.Acceptar\n2.Denegar\n");
        scanf("%d",&opt);
        fflush(stdin);
        if (opt == 1){
            afegir_friend(user,index->sender,head);
            FILE* file = fopen("../friends.txt", "a");
            if (file == NULL) {
                printf("No s'ha pogut obrir el fitxer");
            }
            else{
                fprintf(file, "%s,%s",user->user,index->sender);
            }
            fclose(file);
            index = index->next;
        }
        else if (opt==2){
            user->sol.front = user->sol.front->next;
            if(user->sol.front == user->sol.rear){
                user->sol.rear = NULL;
            }
            index = index->next;
        }
        else printf("Posa 1 o 2\n");

    }
}


Dict* dictionarycheck(Dict* dict, Post* post) {
    char *copy = strdup(post->text);
    char *word = strtok(copy, " ");

    while(word != NULL){
        int found = 0;
        for (int i = 0; i < dict->num; ++i) {
            if (strcmp(dict->key[i], word)== 0) {
                dict->value[i]++;
                found = 1;
                break;
            }
        }
        if (found == 0){
            if (dict->num == MAX_LENGTH){
                printf("El diccionari esta ple i no rebra mes paraules\n");
                break;
            }
            strcpy(dict->key[dict->num],word);
            dict->value[dict->num] = 1;
            dict->num++;
        }
        word = strtok(NULL, " ");
    }
    free(copy);
    return dict;
}
void Dictsort(Dict* dict) {
    for (int i = 0; i < dict->num - 1; i++) {
        for (int j = 0; j < dict->num - i - 1; j++) {
            if (dict->value[j] < dict->value[j + 1]) {
                char tempkey[100];
                strcpy(tempkey, dict->key[j]);
                int tempvalue = dict->value[j];
                dict->value[j] = dict->value[j + 1];
                strcpy(dict->key[j] ,dict->key[j+1]);
                dict->value[j + 1] = tempvalue;
                strcpy(dict->key[j + 1] ,tempkey);
            }
        }
    }
}
Dict readDictFromFile(const char* filename) {
    Dict dict;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("No s'ha pogut obrir el fitxer %s\n", filename);
        dict.num = 0;
        return dict;
    }
    char line[200];

    dict.num = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line,"%[^,],%d",dict.key[dict.num], &dict.value[dict.num]);
        dict.num++;

    }
    fclose(file);
    return dict;
}

void useractions(Dict dict,User* selected, User **head) {
    int opt = -1;
    while (opt == -1) {
        fflush(stdin);
        printf("\n1. Solicitud amistat\n2. Publicacio\n3. Veure publicacions teves\n4.Veure publicacions d'amics\n5. Veure solicituds pendents\n6. Enrere\n");
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
void readPostsFromFile(const char* filename, User **head) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("No s'ha pogut obrir el fitxer %s\n", filename);
        return;
    }
    char line[200];

    while (fgets(line, sizeof(line), file) != NULL) {
        Post *post = (Post *)malloc(sizeof(Post));
        char usern[MAX_LENGTH];
        sscanf(line,"%[^,],%[^|]|%lld)",usern, post->text,&post->t);
        User* user= trobar_user(*head, usern);
        user->posts[user->num_posts] = *post;
        user->num_posts++;
    }
    fclose(file);
}

void readFriendsFromFile(const char* filename, User **head) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("No s'ha pogut obrir el fitxer %s\n", filename);
        return;
    }
    char line[200];

    while (fgets(line, sizeof(line), file) != NULL) {
        char friend1[MAX_LENGTH];
        char friend2[MAX_LENGTH];
        sscanf(line,"%[^,],%s",friend1, friend2);
        User* user = trobar_user(*head, friend1);
        afegir_friend(user, friend2, *head);
    }
    fclose(file);
}
void print_menu(){
    printf("\n1.Insertar usuari nou\n2.Mostrar usuaris existents\n3.Seleccionar usuari\n4.Imprimir paraules mes fetes servir\n5.Sortir\n");
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
void menu(User **head, Dict dict){
    int opt = -1;
    while (opt == -1){
        print_menu();
        fflush(stdin);
        scanf("%d", &opt);
        if (opt == 1){
            //Insertar usuari nou
            usuari_nou(head);
            opt = -1;
        }
        else if (opt == 2){
            User *index = *head;
            if (index == NULL) {
                printf("No hi ha users.\n");
            }
            while (index != NULL) {
                printf("%s\tEdat:%d\tHobbies:%s,%s,%s,%s,%s\n", index->user, index->age, index->hobby[0],index->hobby[1],index->hobby[2],index->hobby[3],index->hobby[4]);
                index = index->next;
            }
            printf("\n");
            opt = -1;
        }
        else if (opt == 3) {
            //Selecció usuari
            char selected_user[MAX_LENGTH];
            printf("Introdueix el nom de l'usuari seleccionat:");
            scanf("%s", selected_user);
            User *selected= trobar_user(*head, selected_user);
            printf("Nom:%s\n", selected->user);
            printf("Edat:%d\n", selected->age);
            printf("Mail:%s\n", selected->mail);
            printf("Ubicacio:%s\n", selected->loc);
            printf("Hobbies:%s,%s,%s,%s,%s\n", selected->hobby[0], selected->hobby[1], selected->hobby[2], selected->hobby[3], selected->hobby[4]);
            //Processos usuari
            useractions(dict,selected,head);
            opt = -1;

        }
        else if (opt == 4){
            dict = readDictFromFile("../dict.txt");
            Dictsort(&dict);
            printf("\nParaules mes usades:\n");
            for (int i = 0; i < 10; ++i) {
                printf("%d)\t%s:\t%d\n", i+1, dict.key[i],dict.value[i]);
            }
            opt = -1;
        }
        else if (opt == 5) {
            //Tancar
            return;
        }
        else {
            printf("Introdueix un valor de 1 a 5\n");
        }
    }
}


int main() {
    User *head = NULL;
    readUsersFromFile("../users.txt", &head);
    readPostsFromFile("../posts.txt", &head);
    readFriendsFromFile("../friends.txt",&head);
    Dict dict = readDictFromFile("../dict.txt");
    menu(&head,dict);
    return 0;
}