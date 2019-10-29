#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../lib/ESLib.h"
#include "ga.h"

// Détermine le score de chaque individu et vérifie si l'un d'eux est au moins ressemblant à 90%
void fitness(population *initPop, mot answer, bool *found, int taillePop) {
    // Remets à 0 le score des individus
    for (int i = 0; i < taillePop; i++)
        initPop[i].fitness = 0;
    // Calcul le score des individus
    for (int i = 0; i < taillePop; i++) {
        for (int j = 0; j < T_MOT; j++)
            if (initPop[i].gene[j] == answer[j])
                initPop[i].fitness += 10;
        // Regarde si un des scores est au moins à 90%
        if (initPop[i].fitness >= 90)
            *found = true;
    }
}

// Affiche dans le terminal la population actuelle
void afficheGeneration(population *initPop, int generation, int taillePop) {
    printf("Génération: %d\n", generation);
    for (int i = 0; i < taillePop; i ++)
        printf("Individu n°%d: %s - Fitness: %d\n", i+1, initPop[i].gene, initPop[i].fitness);
}

// Tirage au sort d'un gène (chaîne) sur une roue biaisée
void frise(population *initPop, mot gene, int total, int taillePop) {
    // Affectation de la valeur aléatoire
    float alea = valeurIntervalleZeroUn()*total, old = 0;
    for (int j = 0; j < taillePop; j++) {
        // Copie le score de l'individu actuel
        old += initPop[j].fitness + 10;
        if (alea <= old) {
            // Dans le cas où la valeur aléatoire est inférieure ou égale au score actuel on copie ce gène (chaîne)
            strcpy(gene, initPop[j].gene);
            break;
        } 
    }
}

// Crée des paires d'individus pour le crossover
void paire(population *initPop, population pairePop[][2], int taillePop) {
    int total = 0;
    for (int i = 0; i < taillePop; i++)
        total += initPop[i].fitness + 10;
    for (int i = 0; i < taillePop-1; i++) {
        frise(initPop, pairePop[i][0].gene, total, taillePop);
        frise(initPop, pairePop[i][1].gene, total, taillePop);
    }
}

// Définit notre nouvel individu
void create(mot gene, population pairePop[][2], int indice, int taillePop) {
    // Détermine la valeur aléatoire qui permettra de sélection un certain pourcentage de chaque individu
    int prop = valeurIntervalleZeroUn()*taillePop;
    // Remplis notre nouvel individu en fonction de la valeur aléatoire précédente
    for (int i = 0; i < prop; i++)
        gene[i] = pairePop[indice][0].gene[i];
    for (int j = prop; j < 10; j++)
        gene[j] = pairePop[indice][1].gene[j];
    gene[10] = '\0';
}

// Applique un crossover sur notre population
void crossover(population *initPop, int taillePop) {
    population pairePop[taillePop-1][2];
    population newPop[taillePop-1];
    // Détermine 9 paires d'individus
    paire(initPop, pairePop, taillePop);
    // Initialise 9 nouveaux individus en fonction des paires précédentes
    for (int i = 0; i < taillePop-1; i++)
        create(newPop[i].gene, pairePop, i, taillePop);
    // Détermine l'individu avec le meilleur score
    int top = 0;
    for (int i = 1; i < 10; i++)
        if (initPop[top].fitness < initPop[i].fitness)
            top = i;
    // Remplace les individus par les nouveaux
    int index = 0;
    for (int k = 0; k < taillePop-1; k++) {
        if (index == top)
            index += 1;
        strcpy(initPop[index].gene, newPop[k].gene);
        index += 1;
    }
}

// Applique une mutation sur la nouvelle population
void mutation(population *initPop, chaine dic, int taillePop) {
    for (int i = 0; i < taillePop; i++) {
        // Une mutation à 20% de chance de s'appliquer sur un des gènes
        if (valeurIntervalleZeroUn() <= 0.2) {
            // Choisis un gène et un mot du dictionnaire aléatoirement
            int index = valeurIntervalleZeroUn()*10;
            int indice = valeurIntervalleZeroUn()*T_DIC;
            initPop[i].gene[index] = dic[indice];
        }
    }
}