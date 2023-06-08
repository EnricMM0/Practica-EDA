#include "main.h"

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

