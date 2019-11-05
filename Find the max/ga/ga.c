#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../lib/ESLib.h"
#include "ga.h"

void stats(population *individu, int taillePop, float intervalMin, float intervalMax) {
    int indexMin = 0, indexMax = 0;
    float moyenne = 0, valeurMin = individu[0].valeur, valeurMax = individu[0].valeur;
    for (int i = 1; i < taillePop; i++) {
        if (individu[indexMin].fitness > individu[i].fitness)
            indexMin = i;
        if (individu[indexMax].fitness < individu[i].fitness)
            indexMax = i;
        if (valeurMin > individu[i].valeur)
            valeurMin = individu[i].valeur;
        if (valeurMax < individu[i].valeur)
            valeurMax = individu[i].valeur;
    }
    for (int i = 0; i < taillePop; i++)
        moyenne += individu[i].fitness;
    printf("Min: %f - Max: %f - Moyenne: %f - Ecart: %f\n", individu[indexMin].fitness, individu[indexMax].fitness, moyenne/taillePop, valeurMax-valeurMin);
}

float computeFitness(float valeur) {
    return valeur*valeur;
}

void initPop(population *individu, int taillePop, float intervalMin, float intervalMax) {
    for (int i = 0; i < taillePop; i++) {
        individu[i].valeur = valeurIntervalleZeroUn()*(intervalMax-intervalMin)+intervalMin;
        individu[i].fitness = computeFitness(individu[i].valeur);
    }
}

void affichePop(population *individu, int taillePop, int generation) {
    printf("\nGénération n°%d:\n", generation);
    for (int i = 0; i < taillePop; i++)
        printf("Individu n°%d: Valeur (%f) Fitness (%f)\n", i, individu[i].valeur, individu[i].fitness);
}

void frise(population *individu, population paire[][2], int taillePop) {
    float total = 0;
    for (int i = 0; i < taillePop; i++)
        total += individu[i].fitness;
    float alea = 0, old = 0;
    for (int i = 0; i < taillePop-1; i++) {
        for (int j = 0; j < 2; j++) {
            alea = valeurIntervalleZeroUn()*total;
            old = 0;
            for (int k = 0; k < taillePop; k++) {
                old += individu[k].fitness;
                if (alea <= old) {
                    paire[i][j] = individu[k];
                    break;
                }
            }
        }
    }
}

void crossover(population *individu, int taillePop, float intervalMin, float intervalMax) {
    int top = 0, index = 0;
    float alea = 0;
    population paire[taillePop-1][2];
    population nouvelle[taillePop-1];
    frise(individu, paire, taillePop);
    for (int i = 0; i < taillePop-1; i++) {
        alea = valeurIntervalleZeroUn();
        nouvelle[i].valeur = alea*paire[i][0].valeur+(1-alea)*paire[i][1].valeur;
        nouvelle[i].valeur = (nouvelle[i].valeur > intervalMax) ? intervalMax : nouvelle[i].valeur;
        nouvelle[i].valeur = (nouvelle[i].valeur < intervalMin) ? intervalMin : nouvelle[i].valeur;
        nouvelle[i].fitness = computeFitness(nouvelle[i].valeur);
    }
    for (int i = 1; i < taillePop; i++)
        if (individu[top].fitness < individu[i].fitness)
            top = i;
    for (int i = 0; i < taillePop-1; i++) {
        if (i == top)
            index += 1;
        individu[index] = nouvelle[i];
        index += 1;
    }
}

void mutation(population *individu, int taillePop, float intervalMin, float intervalMax, float chanceMutation) {
    for (int i = 0; i < taillePop; i++) {
        if (valeurIntervalleZeroUn() <= (chanceMutation/100)) {
            individu[i].valeur = valeurIntervalleZeroUn()*(intervalMax-intervalMin)+intervalMin;
            individu[i].fitness = computeFitness(individu[i].valeur);
        }
    }
}