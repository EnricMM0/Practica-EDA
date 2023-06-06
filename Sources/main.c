#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_LENGTH 20
#define MAX_USERS 50
#define MAX_FRIENDS 50
#define MAX_POSTS 100
#define MAX_LENGTH_POST 120
#define MAX_WORDS 100
typedef struct {
    char text[MAX_LENGTH_POST];
    time_t t;
} Post;

typedef struct {
    int data;
    struct Node* next;
} Node;
typedef struct  {
    Node* front;
    Node* rear;
} Queue ;



typedef struct{
    char user[MAX_LENGTH];
    int age;
    char mail[MAX_LENGTH];
    char loc[MAX_LENGTH];
    char hobby[5][MAX_LENGTH];
    int friends[MAX_FRIENDS];
    int num_friends;
    Post posts[MAX_POSTS];
    int num_posts;
    Queue* sol;
} User;
typedef struct {
    int num;
    User users[MAX_USERS];
} List;
typedef struct{
    char key[MAX_LENGTH][MAX_WORDS];
    int value[MAX_WORDS];
    int num;
} Dict;

//funcions queue
int isEmpty(Queue* queue) {
    return (queue->front == NULL);
}
Queue* createQueue() {
    Queue* queue = (Queue*) malloc(sizeof(Queue*));
    queue->front = queue->rear = NULL;
    return queue;
}
void enqueue(Queue* queue, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    if (isEmpty(queue)) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}
int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return -1;
    }
    Node* temp = queue->front;
    int data = temp->data;
    queue->front = queue->front->next;
    free(temp);
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    return data;
}
void proces_solicitud(Queue* queue) {
    if (isEmpty(queue)) {
        printf("No tens solicituds pendents.\n");
        return;
    }
    Node* temp = queue->front;
    printf("Solicituds: ");
    while (temp != NULL) {
        printf("%d ", temp->data);

        scanf;
        temp = temp->next;
    }
    printf("\n");
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
                printf("El diccionari esta ple i no rebra mes paraules");
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
int useractions(List list, Dict dict,int selected_user_index) {
    int opt = -1;
    while (opt == -1){
        fflush(stdin);
        printf("\n1. Solicitud amistat\n2. Publicacio\n3. Enrere\n");
        scanf("%d", &opt);
        if (opt == 1) {
            char friend[MAX_LENGTH];
            printf("Introdueix el usuari de l'amic:");
            scanf("%s", friend);
            int found_user = -1;
            for (int i = 0;i < list.num; i++) {
                if (strcmp(list.users[i].user, friend) == 0) {
                    found_user = i;
                    break;
                }
            }
            if (found_user != -1) {
                //solicitud amic
                if (list.users[selected_user_index].num_friends >= MAX_FRIENDS) {
                    printf("Has arribat al limit de amics");
                }
                else{
                    enqueue(list.users[found_user].sol, selected_user_index);
                    printf("Solicitud amistat enviada.\n");
                }
            } else {
                printf("No s'ha trobat l'usuari.\n");
            }
            opt = -1;
        }
        else if (opt == 2) {
            if (list.users[selected_user_index].num_posts >= MAX_POSTS) {
                printf("Has arribat al límit de publicacions.\n");
            } else {
                Post post;
                time(&post.t);
                printf("Temps:%s", ctime(&post.t));
                scanf("%s", post.text);
                list.users[selected_user_index].posts[list.users[selected_user_index].num_posts] = post;
                list.users[selected_user_index].num_posts++;
                dictionarycheck(&dict, &post);
                FILE *file = fopen("../dict.txt", "w");
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
        else{
            return 0;
        }
    }
}
List readUsersFromFile(const char* filename) {
    List *list = malloc(sizeof(List));
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("No s'ha pogut obrir el fitxer %s\n", filename);
        list->num = 0;
        return *list;
    }
    char line[200];

    list->num = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        User user;
        sscanf(line,"%[^,],%d,%[^,],%[^,],(%[^,],%[^,],%[^,],%[^,],%[^)])",user.user, &user.age, user.mail, user.loc,user.hobby[0],user.hobby[1],user.hobby[2],user.hobby[3],user.hobby[4] );
        list->users[list->num] = user;
        list->num++;

    }

    fclose(file);
    return *list;
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
void print_menu(){
    printf("\n1.Insertar usuari nou\n2.Mostrar usuaris existents\n3.Seleccionar usuari\n4.Sortir\n");
}
User usuari_nou() {
    User newuser;
    printf("Usuari:");
    scanf("%s", newuser.user);
    printf("\nEdat:");
    scanf("%d", &newuser.age);
    printf("\nMail:");
    scanf(" %[^\n]", newuser.mail);
    printf("\nUbicacio:");
    scanf(" %[^\n]", newuser.loc);
    printf("\n5 Hobbies:");
    for (int j = 0; j < 5; j++) {
        scanf("%s", newuser.hobby[j]);
    }
    for (int j = 0; j < MAX_FRIENDS; j++) {
        newuser.friends[j] = -1;
    }
    return newuser;
}
int menu(List list, Dict dict){

    int opt = -1;
    while (opt == -1){
        print_menu();
        fflush(stdin);
        scanf("%d", &opt);
        if (opt == 1){
            //Insertar usuari nou
            User newuser = usuari_nou();
            FILE* file = fopen("../users.txt","a");
            if (file == NULL) {
                printf("No s'ha pogut obrir el fitxer");
            }
            else{
                fprintf(file, "\n%s,%d,%s,%s,(%s,%s,%s,%s,%s)",newuser.user,newuser.age,newuser.mail,newuser.loc,newuser.hobby[0],newuser.hobby[1],newuser.hobby[2],newuser.hobby[3],newuser.hobby[4]);

                fclose(file);
                list = readUsersFromFile("../users.txt");
            }
            opt = -1;
        }
        else if (opt == 2){
            for (int i = 0; i < list.num-1; i++){
                printf("\n%s\tEdat:%d\tHobbies:%s,%s,%s,%s,%s", list.users[i].user, list.users[i].age, list.users[i].hobby[0],list.users[i].hobby[1],list.users[i].hobby[2],list.users[i].hobby[3],list.users[i].hobby[4]);
            }
            printf("\n");
            opt = -1;
        }
        else if (opt == 3) {
            //Selecció usuari
            char selected_user[MAX_LENGTH];
            printf("Introdueix el nom de l'usuari seleccionat:");
            scanf("%s", selected_user);
            int found_user = -1;
            for (int i = 0; i < list.num; i++) {
                if (strcmp(list.users[i].user, selected_user) == 0) {
                    found_user = i;
                    break;
                }
            }
            if (found_user == -1) {
                printf("No s'ha trobat l'usuari seleccionat\n");
                opt = -1;
            } else {
                printf("Nom:%s\n", list.users[found_user].user);
                printf("Edat:%d\n", list.users[found_user].age);
                printf("Mail:%s\n", list.users[found_user].mail);
                printf("Ubicacio:%s\n", list.users[found_user].loc);
                printf("Hobbies:%s,%s,%s,%s,%s\n", list.users[found_user].hobby[0], list.users[found_user].hobby[1], list.users[found_user].hobby[2], list.users[found_user].hobby[3], list.users[found_user].hobby[4]);
                int selected_user_index = found_user;
                //Processos usuari
                useractions(list,dict,selected_user_index);
                opt = -1;
            }
        }
        else if (opt == 4) {
            //Tancar
            return 0;
        }
        else {
            printf("Introdueix un valor de 1 a 4\n");
        }
        printf("Hola");
    }
    return 0;
}


int main() {
    List list = readUsersFromFile("../users.txt");
    Dict dict = readDictFromFile("../dict.txt");
    menu(list,dict);
    return 0;
}