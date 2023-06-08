#include "main.h"

void envia_solicitud(User* enviador, User* recipient){
    Soli* newSoli = (Soli*) malloc(sizeof(Soli)); //creem solicitud nova i posem en cua del recipient
    strcpy(newSoli->sender,enviador->user);
    newSoli->next = NULL;
    if (recipient->sol.front == NULL){ //posem en primera posició
        recipient->sol.front = newSoli;
        recipient->sol.rear = newSoli;
    }
    else{//posem en última posició
        recipient->sol.rear->next = newSoli;
        recipient->sol.rear= newSoli;
    }
}

void afegir_friend(User *user,char friend[MAX_LENGTH], User *head){
    //1ra direcció: user afegeix friend
    Friend *new_friend = (Friend *) malloc(sizeof(Friend));
    strcpy(new_friend->user,friend); //guardem usuari d'amic
    new_friend->next = NULL; //amic serà últim, no tindrà next
    if (user->friends.head == NULL) {
        user->friends.head = new_friend; //amic es guarda com a primer
    } else {
        Friend *index = user->friends.head;
        while (index->next != NULL) {
            index = index->next; //busquem últim amic
        }
        index->next = new_friend; //amic es guarda com a últim
    }
    //2na direcció: friend afegeix user
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
    Soli *index = user->sol.front; //index que recorrerà cua
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
        fflush(stdin); //buidem input consola
        if (opt == 1){
            afegir_friend(user,index->sender,head); // afegim amic
            FILE* file = fopen("../friends.txt", "a");
            if (file == NULL) {
                printf("No s'ha pogut obrir el fitxer");
            }
            else{
                fprintf(file, "%s,%s\n",user->user,index->sender); // escribim amics en fitxer text
            }
            fclose(file);
            user->sol.front = user->sol.front->next; //esborrem solicitud processada
            if(user->sol.front == user->sol.rear){
                user->sol.rear = NULL;
            }
            index = index->next; //index al següent
        }
        else if (opt==2){
            user->sol.front = user->sol.front->next; //esborrem solicitud processada
            if(user->sol.front == user->sol.rear){
                user->sol.rear = NULL;
            }
            index = index->next; //index al següent
        }
        else printf("Posa 1 o 2\n");

    }
}

void readFriendsFromFile(const char* filename, User **head) {
    FILE* file = fopen(filename, "r"); //Obrim fitxer mode lectura
    if (file == NULL) {
        printf("No s'ha pogut obrir el fitxer %s\n", filename);
        return;
    }
    char line[200]; //búfer de línea

    while (fgets(line, sizeof(line), file) != NULL) {
        char friend1[MAX_LENGTH];
        char friend2[MAX_LENGTH];
        sscanf(line,"%[^,],%s",friend1, friend2); //scanegem cada línea
        User* user = trobar_user(*head, friend1);
        afegir_friend(user, friend2, *head); //afegim les dos persones com a amics
    }
    fclose(file);
}
