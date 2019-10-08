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

void fitness(population initPop[10], mot answer, bool found) {
    for (int i = 0; i < 10; i ++) {
        for (int j = 0; j < T_MOT; j ++)
            if (initPop[i].gene[j] == answer[j])
                initPop[i].fitness += 1;
        if (initPop[i].fitness == 10)
            found = true;
    }
}

void afficheGeneration(population initPop[10], int generation) {
    printf("Génération: %d\n", generation);
    for (int i = 0; i < 10; i ++)
        printf("Individu n°%d: %s - Fitness: %d\n", i+1, initPop[i].gene, initPop[i].fitness);
}

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

    fitness(initPop, answer, found);
    //while (!found) {
        afficheGeneration(initPop, generation);
        generation += 1;
        
    //}
}