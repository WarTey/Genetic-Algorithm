#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "../lib/GfxLib.h"
#include "../lib/ESLib.h"
#include "ga.h"

// Détermine la distance de chaque individu
void fitness(individu tabIndividu[10], ville tabVille[10]) {
    // Remets à 0 le score des individus
    for (int i = 0; i < 10; i++)
        tabIndividu[i].fitness = 0;
    // Calcul la distance des individus
    for (int i = 0; i < 10; i++) {
        for (int j = 1; j < 10; j++)
            tabIndividu[i].fitness += sqrt(pow(tabVille[tabIndividu[i].chemin[j]].x - tabVille[tabIndividu[i].chemin[j-1]].x, 2) + pow(tabVille[tabIndividu[i].chemin[j]].y - tabVille[tabIndividu[i].chemin[j-1]].y, 2));
        tabIndividu[i].fitness += sqrt(pow(tabVille[tabIndividu[i].chemin[0]].x - tabVille[tabIndividu[i].chemin[9]].x, 2) + pow(tabVille[tabIndividu[i].chemin[0]].y - tabVille[tabIndividu[i].chemin[9]].y, 2));
    }
}

// Affiche dans le terminal la population actuelle
void afficheGeneration(individu tabIndividu[10], int generation) {
    printf("Génération: %d\n", generation);
    for (int i = 0; i < 10; i++) {
        printf("Individu n°%d: ", i+1);
        for (int j = 0; j < 10; j++)
            printf("%d ", tabIndividu[i].chemin[j]);
        printf("- Fitness: %f\n", tabIndividu[i].fitness);
    }
}