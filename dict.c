#include "main.h"
Dict* dictionarycheck(Dict* dict, Post* post) {
    char *copy = strdup(post->text);
    char *word = strtok(copy, " "); //tokenització de text

    while(word != NULL){ //mirem totes paraules dins del post
        int found = 0;
        if (dict->num == MAX_WORDS){
            printf("El diccionari esta ple i no rebra mes paraules\n");
            break;
        }
        for (int i = 0; i < dict->num; ++i) {
            if (strcmp(dict->key[i], word)== 0) { //busquem paraula en diccionari
                dict->value[i]++;
                found = 1;
                break;
            }
        }
        if (found == 0){
            //si no hem trobat la paraula, afegim un element del diccionari, amb key=paraula i valor=1
            strcpy(dict->key[dict->num],word);
            dict->value[dict->num] = 1;
            dict->num++;
        }
        word = strtok(NULL, " "); // següent paraula
    }
    free(copy);
    return dict;
}
void Dictsort(Dict* dict) { //ordenem diccionari amb bubblesort, números més grans primer.
    for (int i = 0; i < dict->num - 1; i++) {
        for (int j = 0; j < dict->num - i - 1; j++) {
            if (dict->value[j] < dict->value[j + 1]) { //si estàn en l'ordre incorrecte, cambiem j i j+1 del dict
                char tempkey[100]; //valors temporals per intercambiar valors
                int tempvalue = dict->value[j];
                strcpy(tempkey, dict->key[j]);
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
    FILE* file = fopen(filename, "r"); //Obrim fitxer en mode lectura
    if (file == NULL) {
        printf("No s'ha pogut obrir el fitxer %s\n", filename);
        dict.num = 0;
        return dict;
    }
    char line[200]; //búfer de línea

    dict.num = 0;
    while (fgets(line, sizeof(line), file) != NULL) { //llegim cada línea i assignem als llocs corresponents
        sscanf(line,"%[^,],%d",dict.key[dict.num], &dict.value[dict.num]);
        dict.num++;
    }
    fclose(file);
    return dict;
}


