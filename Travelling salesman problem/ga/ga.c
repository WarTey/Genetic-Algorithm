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

// Affiche, dans le terminal, la population actuelle
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
void frise(individu tabIndividu[10], individu *paireIndividu, float total) {
    // Affectation de la valeur aléatoire
    float alea = valeurIntervalleZeroUn()*total, old = 0;
    for (int j = 0; j < 10; j++) {
        // Copie la distance de l'individu actuel
        old += 1/tabIndividu[j].fitness;
        if (alea <= old) {
            // Dans le cas où la valeur aléatoire est inférieure ou égale au score actuel, on garde ces chemins
            for (int i = 0; i < 10; i++)
                paireIndividu->chemin[i] = tabIndividu[j].chemin[i];
            break;
        } 
    }
}

// Met en place des paires d'individus pour le crossover
void paire(individu tabIndividu[10], individu paireIndividu[9][2]) {
    float total = 0;
    // Détermine le score total mais inversé pour que les villes avec la distance la plus courte
    // est plus de chance d'être sélectionnées
    for (int i = 0; i < 10; i++)
        total += 1/tabIndividu[i].fitness;
    for (int i = 0; i < 9; i++) {
        frise(tabIndividu, &(paireIndividu[i][0]), total);
        frise(tabIndividu, &(paireIndividu[i][1]), total);
    }
}

// Définit notre nouvel individu
void create(individu newIndividu[9], individu paireIndividu[9][2], int indice) {
    // Détermine la valeur aléatoire qui permettra de sélection un certain pourcentage de chaque individu
    int prop = valeurIntervalleZeroUn()*10;
    // Remplit notre nouvel individu en fonction de la valeur aléatoire précédente (avec la paire 1)
    for (int i = 0; i < prop; i++)
        newIndividu[indice].chemin[i] = paireIndividu[indice][0].chemin[i];
    // Le reste du tableau correspond au chemin de l'autre individu (paire 2) mais qui ne sont pas déjà présent
    for (int i = 0; i < 10; i++) {
        if (prop < 10) {
            int temoin = true;
            // Détermine si la valeure est déjà présente ou non
            for (int j = 0; j < prop; j++) {
                if (paireIndividu[indice][1].chemin[i] == newIndividu[indice].chemin[j]) {
                    temoin = false;
                    break;
                }
            }
            // Si elle ne l'est pas, alors l'ajoute
            if (temoin) {
                newIndividu[indice].chemin[prop] = paireIndividu[indice][1].chemin[i];
                prop += 1;
            }
        }
    }
}

// Applique un crossover sur notre population
void crossover(individu tabIndividu[10]) {
    individu paireIndividu[9][2];
    individu newIndividu[9];
    // Détermine 9 paires d'individus
    paire(tabIndividu, paireIndividu);
    // Initialise 9 nouveaux individus en fonction des paires précédentes
    for (int i = 0; i < 9; i++)
        create(newIndividu, paireIndividu, i);
    // Détermine l'individu avec la distance la plus courte
    int top = 0;
    for (int i = 1; i < 10; i++)
        if (tabIndividu[top].fitness > tabIndividu[i].fitness)
            top = i;
    // Remplace les individus par les nouveaux (sauf celui avec la plus petite distance)
    int index = 0;
    for (int k = 0; k < 9; k++) {
        if (index == top)
            index += 1;
        tabIndividu[index] = newIndividu[k];
        index += 1;
    }
}

// Applique une mutation sur la nouvelle population
void mutation(individu tabIndividu[10]) {
    for (int i = 0; i < 10; i++) {
        // Une mutation à 1% de chance de s'appliquer
        if (valeurIntervalleZeroUn() <= 0.01) {
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