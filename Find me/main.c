#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lib/ESLib.h"

#define T_DIC 27
#define T_MOT 10

typedef char chaine[T_DIC];
typedef char mot[T_MOT+1];

typedef struct population {
    mot gene;
    int fitness;
} population;

int main(void) {
    int generation = 0, indice = 0;
    bool found = false;
    chaine dic = "abcdefghijklmnopqrstuvwxyz ";
    mot answer = "";

    printf("Saisir un chaîne de 10 caractères :");
    scanf("%s", answer);
    answer[T_MOT] = '\0';

    population initPop[10];
    for (int i = 0; i < 10; i ++) {
        for (int j = 0; j < T_MOT; j ++) {
            indice = valeurIntervalleZeroUn()*T_DIC;
            initPop[i].gene[j] = dic[indice];
        }
        initPop[i].gene[T_MOT] = '\0';
        initPop[i].fitness = 0;
    }
}