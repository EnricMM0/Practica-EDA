#include "main.h"
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
