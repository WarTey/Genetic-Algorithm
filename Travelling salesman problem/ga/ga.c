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

// Tirage au sort sur une roue biaisée
void frise(individu tabIndividu[10], individu *paireIndividu, int total) {
    // Affectation de la valeur aléatoire
    int alea = valeurIntervalleZeroUn()*total, old = 0;
    for (int j = 0; j < 10; j++) {
        // Copie la distance de l'individu actuel
        old += 1/tabIndividu[j].fitness;
        if (alea <= old) {
            // Dans le cas où la valeur aléatoire est inférieure ou égale au score actuel on copie ces chemins
            for (int i = 0; i < 10; i++)
                paireIndividu->chemin[i] = tabIndividu[j].chemin[i];
            break;
        } 
    }
}

// Crée des paires d'individus pour le crossover
void paire(individu tabIndividu[10], individu paireIndividu[5][2]) {
    float total = 0;
    for (int i = 0; i < 10; i++)
        total += 1/tabIndividu[i].fitness;
    for (int i = 0; i < 5; i++) {
        frise(tabIndividu, &(paireIndividu[i][0]), total);
        frise(tabIndividu, &(paireIndividu[i][1]), total);
    }
}

// Définit notre nouvel individu
void create(individu newIndividu[5], individu paireIndividu[5][2], int indice) {
    // Détermine la valeur aléatoire qui permettra de sélection un certain pourcentage de chaque individu
    int prop = valeurIntervalleZeroUn()*10;
    // Remplis notre nouvel individu en fonction de la valeur aléatoire précédente
    for (int i = 0; i < prop; i++)
        newIndividu[indice].chemin[i] = paireIndividu[indice][0].chemin[i];
    // Modifie le tableau des chemins pour éviter les doublons
    for (int i = 0; i < prop; i++) {
        for (int j = i+1; j < 10; j++) {
            if (paireIndividu[indice][0].chemin[i] == paireIndividu[indice][1].chemin[j]) {
                int temp = paireIndividu[indice][1].chemin[j];
                paireIndividu[indice][1].chemin[j] = paireIndividu[indice][1].chemin[i];
                paireIndividu[indice][1].chemin[i] = temp;
                break;
            }
        }
    }
    // Copie le reste du chemin
    for (int i = prop; i < 10; i++)
        newIndividu[indice].chemin[i] = paireIndividu[indice][1].chemin[i];
}

// Applique un crossover sur notre population
void crossover(individu tabIndividu[10]) {
    individu paireIndividu[5][2];
    individu newIndividu[5];
    // Détermine 5 paires d'individus
    paire(tabIndividu, paireIndividu);
    // Initialise 5 nouveaux individus en fonction des paires précédentes
    for (int i = 0; i < 5; i++)
        create(newIndividu, paireIndividu, i);
    int index = 0;
    // Remplace les individus avec la distance la plus haute par les nouveaux individus
    for (int k = 0; k < 5; k++) {
        index = 0;
        for (int i = 1; i < 10; i++)
            if (tabIndividu[index].fitness < tabIndividu[i].fitness)
                index = i;
        tabIndividu[index] = newIndividu[k];
    }
}

// Applique une mutation sur la nouvelle population
void mutation(individu tabIndividu[10]) {
    for (int i = 0; i < 10; i++) {
        // Une mutation à 10% de chance de s'appliquer
        if (valeurIntervalleZeroUn() <= 0.1) {
            // Choisis un chemin aléatoire et une position aléatoire
            int chemin = valeurIntervalleZeroUn()*10;
            int index = valeurIntervalleZeroUn()*10;
            for (int j = 0; j < 10; j++) {
                if (tabIndividu[i].chemin[j] == chemin) {
                    int temp = tabIndividu[i].chemin[j];
                    tabIndividu[i].chemin[j] = tabIndividu[i].chemin[index];
                    tabIndividu[i].chemin[index] = chemin;
                }
            }
        }
    }
}