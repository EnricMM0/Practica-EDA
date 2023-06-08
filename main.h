#ifndef EDA2_MAIN_H
#define EDA2_MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_LENGTH 20
#define MAX_POSTS 100
#define MAX_LENGTH_POST 120
#define MAX_WORDS 100

//estructures
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

//funcions
//usuari.c
User* trobar_user(User *head, char user[MAX_LENGTH]);
void useractions(Dict dict,User* selected, User **head);
void readUsersFromFile(const char* filename, User **head);
void usuari_nou(User **head);

//dict.c
Dict* dictionarycheck(Dict* dict, Post* post);
void Dictsort(Dict* dict);
Dict readDictFromFile(const char* filename);

//friends.c
void envia_solicitud(User* enviador, User* recipient);
void afegir_friend(User *user,char friend[MAX_LENGTH], User *head);
void proces_solicitud(User *user,User* head);
void readFriendsFromFile(const char* filename, User **head);

//posts.c
void readPostsFromFile(const char* filename, User **head);
#endif //EDA2_MAIN_H