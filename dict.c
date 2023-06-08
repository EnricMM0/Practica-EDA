#include "main.h"
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
            if (dict->num == MAX_WORDS){
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


