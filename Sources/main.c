#include <stdio.h>

void print_menu(){
    printf("1.Insertar usuari nou\n2.Mostrar usuaris existents\n3.Seleccionar usuari\n4.Sortir\n");
}

int menu(){
    int opt = -1;
    while (opt == -1){
        print_menu();
        scanf("%d", &opt);
        if (opt == 1){
            //Usuari nou
        }
        else if (opt == 2){
            //Llista usuaris
        }
        else if (opt == 3){
            //Selecció usuari
        }
        else if (opt == 4) {
            //Tancar
        }
        else {
            printf("Introdueix un valor de 1 a 4");
            opt = -1;
        }

    }
}

int main() {
    menu();
    return 0;
}
