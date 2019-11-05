#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../lib/ESLib.h"
#include "ga.h"

// Donne des statistiques sur la population actuelle
void stats(population *individu, int taillePop, float intervalMin, float intervalMax) {
    int indexMin = 0, indexMax = 0;
    float moyenne = 0, valeurMin = individu[0].valeur, valeurMax = individu[0].valeur;
    for (int i = 1; i < taillePop; i++) {
        // Détermine l'individu avec le plus petit score
        if (individu[indexMin].fitness > individu[i].fitness)
            indexMin = i;
        // Détermine l'individu avec le plus grand score
        if (individu[indexMax].fitness < individu[i].fitness)
            indexMax = i;
        // Détermine l'individu avec la plus petite valeur
        if (valeurMin > individu[i].valeur)
            valeurMin = individu[i].valeur;
        // Détermine l'individu avec la plus grande valeur
        if (valeurMax < individu[i].valeur)
            valeurMax = individu[i].valeur;
    }
    // Détermine la moyenne des scores des individus
    for (int i = 0; i < taillePop; i++)
        moyenne += individu[i].fitness;
    // Affiche le score min et max, la moyenne des scores et l'écart entre le plus petit individu et le plus grand
    printf("Min: %f - Max: %f - Moyenne: %f - Ecart: %f\n", individu[indexMin].fitness, individu[indexMax].fitness, moyenne/taillePop, valeurMax-valeurMin);
}

// Détermine le score d'un individu
float computeFitness(float valeur) {
    return valeur*valeur;
}

// Initialise notre population
void initPop(population *individu, int taillePop, float intervalMin, float intervalMax) {
    for (int i = 0; i < taillePop; i++) {
        // Définit une valeur aléatoire compris dans l'intervalle
        individu[i].valeur = valeurIntervalleZeroUn()*(intervalMax-intervalMin)+intervalMin;
        // Calcul le score de cet individu
        individu[i].fitness = computeFitness(individu[i].valeur);
    }
}

// Affiche les infos de nos individus dans le terminal
void affichePop(population *individu, int taillePop, int generation) {
    printf("\nGénération n°%d:\n", generation);
    for (int i = 0; i < taillePop; i++)
        printf("Individu n°%d: Valeur (%f) Fitness (%f)\n", i, individu[i].valeur, individu[i].fitness);
}

// Utilise le principe de la roue biaisée pour sélectionner nos paires
void frise(population *individu, population paire[][2], int taillePop) {
    float total = 0;
    // Détermine le score total
    for (int i = 0; i < taillePop; i++)
        total += individu[i].fitness;
    float alea = 0, old = 0;
    for (int i = 0; i < taillePop-1; i++) {
        for (int j = 0; j < 2; j++) {
            alea = valeurIntervalleZeroUn()*total;
            old = 0;
            for (int k = 0; k < taillePop; k++) {
                old += individu[k].fitness;
                // Si la valeur aléatoire est comprise entre le score de l'individu précédent et de 'lindividu actuel alors on le garde
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
    // Détermine nos paires à l'aide de la roue biaisée
    frise(individu, paire, taillePop);
    // Applique un 'crossover' avec les individus contenu dans le tableau 'paires' pour initialiser notre nouvelle population
    for (int i = 0; i < taillePop-1; i++) {
        alea = valeurIntervalleZeroUn();
        // Sélectionne un certain pourcentage de la valeur de chaque individu
        nouvelle[i].valeur = alea*paire[i][0].valeur+(1-alea)*paire[i][1].valeur;
        // Vérifie qu'il n'est pas dépassé l'intervalle max et min
        nouvelle[i].valeur = (nouvelle[i].valeur > intervalMax) ? intervalMax : nouvelle[i].valeur;
        nouvelle[i].valeur = (nouvelle[i].valeur < intervalMin) ? intervalMin : nouvelle[i].valeur;
        // Calcul le score
        nouvelle[i].fitness = computeFitness(nouvelle[i].valeur);
    }
    // Recherche notre meilleur individu pour le conserver
    for (int i = 1; i < taillePop; i++)
        if (individu[top].fitness < individu[i].fitness)
            top = i;
    // Remplace nos individus par la nouvelle population (sauf le meilleur)
    for (int i = 0; i < taillePop-1; i++) {
        if (i == top)
            index += 1;
        individu[index] = nouvelle[i];
        index += 1;
    }
}

// Applique une mutation sur un gène d'un individu
void mutation(population *individu, int taillePop, float intervalMin, float intervalMax, float chanceMutation) {
    for (int i = 0; i < taillePop; i++) {
        // Une mutation a 'chanceMutation'% de chance de se produire
        if (valeurIntervalleZeroUn() <= (chanceMutation/100)) {
            // Détermine une nouvelle valeur toujours comprise dans l'intervalle
            individu[i].valeur = valeurIntervalleZeroUn()*(intervalMax-intervalMin)+intervalMin;
            // Recalcul son score
            individu[i].fitness = computeFitness(individu[i].valeur);
        }
    }
}