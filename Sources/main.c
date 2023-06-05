#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_LENGTH 20
#define MAX_USERS 50
#define MAX_FRIENDS 50
#define MAX_POSTS 100
#define MAX_LENGTH_POST 120
typedef struct {
    char text[MAX_LENGTH_POST];
    time_t t;
} Post;

typedef struct{
    char user[MAX_LENGTH];
    int age;
    char mail[MAX_LENGTH];
    char loc[MAX_LENGTH];
    char hobby[5][MAX_LENGTH];
    int friends[MAX_FRIENDS];
    Post posts[MAX_POSTS];
    int num_posts;
} User;
typedef struct {
    int num;
    User users[MAX_USERS];
} List;
typedef struct{
    char key[100][MAX_LENGTH];
    int value[100];
    int num;
} Dict;

Dict* dictionarycheck(Dict* dict, Post* post) {
    char word[MAX_LENGTH];
    int lenword = 0;
    for (int i = 0; i < MAX_LENGTH_POST; ++i) {
        if (post->text[i] != ' ' && post->text[i] != '\0'&& post->text[i] != '\n') {
            // lletra
            word[lenword] = post->text[i];
            lenword++;
        } else {
            // espai o final
            if (lenword > 0) {
                // comprovem dict
                int found = 0;
                for (int j = 0; j < dict->num; ++j) {
                    if (strcmp(dict->key[j], word) == 0) {
                        dict->value[j]++;
                        found = 1;
                        break;
                    }
                }
                if (found == 0) {
                    strcpy(dict->key[dict->num], word);
                    dict->value[dict->num] = 1;
                    dict->num++;
                }
                // buidem word
                memset(word, 0, sizeof(char) * MAX_LENGTH);
                lenword = 0;
            }
        }
    }
    return dict;
}

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
        sscanf(line,"%[^,],%d",dict.key[dict.num], dict.value);
        dict.num++;

    }
    fclose(file);
    return dict;
}
void print_menu(){
    printf("\n1.Insertar usuari nou\n2.Mostrar usuaris existents\n3.Seleccionar usuari\n4.Sortir\n");
}
int menu(List list, Dict dict){
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
            for (int i = 0; i < list.num-1; i++){
                printf("\n%s\tEdat:%d\tHobbies:%s,%s,%s,%s,%s", list.users[i].user, list.users[i].age, list.users[i].hobby[0],list.users[i].hobby[1],list.users[i].hobby[2],list.users[i].hobby[3],list.users[i].hobby[4]);
            }
            printf("\n");
            return 0;
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
            } else {
                printf("Nom:%s\n", list.users[found_user].user);
                printf("Edat:%d\n", list.users[found_user].age);
                printf("Mail:%s\n", list.users[found_user].mail);
                printf("Ubicacio:%s\n", list.users[found_user].loc);
                printf("Hobbies:%s,%s,%s,%s,%s\n", list.users[found_user].hobby[0], list.users[found_user].hobby[1], list.users[found_user].hobby[2], list.users[found_user].hobby[3], list.users[found_user].hobby[4]);
                int selected_user_index = found_user;
                //Processos usuari
                printf("\n1. Solicitud amistat\n2. Publicacio\n3. Enrere\n");
                opt = -1;
                while (opt == -1) {
                    scanf("%d", &opt);
                    if (opt == 1) {
                        char friend[MAX_LENGTH];
                        scanf("%s", friend);
                        int found_user = -1;
                        for (int i = 0; i < list.num; i++) {
                            if (strcmp(list.users[i].user, friend) == 0) {
                                found_user = i;
                                break;
                            }
                        }
                        if (found_user != -1) {
                            int newfriend = 0;
                            int i = 0;
                            while (newfriend == 0 && i < MAX_FRIENDS) {
                                if (list.users[selected_user_index].friends[i] == -1) {
                                    //solicitud amic
                                    newfriend = 1;
                                    list.users[selected_user_index].friends[i] = found_user;
                                    printf("Solicitud amistat enviada.\n");
                                    break;
                                }
                                i += 1;
                            }
                            if (newfriend == 0) {
                                printf("Ja has arribat al límit d'amistats.\n");
                            }
                        } else {
                            printf("No s'ha trobat l'usuari.\n");
                        }
                    } else if (opt == 2) {
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
                            FILE* file = fopen("../dict.txt", "w");
                            if (file == NULL) {
                                printf("No s'ha pogut obrir el fitxer.\n");
                            } else {
                                for (int i = 0; i < dict.num; ++i) {
                                    fprintf(file, "%s,%d\n", dict.key[i], dict.value[i]);
                                }
                                fclose(file);
                            }
                            printf("Publicacio completada.\n");
                            break;
                        }
                    } else if (opt == 3) {
                        return 0;
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
    Dict dict = readDictFromFile("../dict.txt");
    int stop = 0;
    while (stop==0) {
        stop = menu(list,dict);
    }
    return 0;
}