#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "lib/ESLib.h"

// Taille du dictionnaire et du mot à saisir
#define T_DIC 27
#define T_MOT 10

typedef char chaine[T_DIC];
typedef char mot[T_MOT+1];

typedef struct population {
    mot gene;
    int fitness;
} population;

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
    int alea = valeurIntervalleZeroUn()*total, old = 0;
    for (int j = 0; j < taillePop; j++) {
        // Copie le score de l'individu actuel
        old += initPop[j].fitness;
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
        total += initPop[i].fitness;
    for (int i = 0; i < taillePop/2; i++) {
        frise(initPop, pairePop[i][0].gene, total, taillePop);
        frise(initPop, pairePop[i][1].gene, total, taillePop);
    }
}

// Définit notre nouvel individu
void create(mot gene, population pairePop[][2], int indice, int taillePop) {
    // Détermine la valeur aléatoire qui permettra de sélection un certain pourcentage de chaque individu
    int prop = valeurIntervalleZeroUn()*(taillePop/2);
    // Remplis notre nouvel individu en fonction de la valeur aléatoire précédente
    for (int i = 0; i < prop; i++)
        gene[i] = pairePop[indice][0].gene[i];
    for (int j = prop; j < 10; j++)
        gene[j] = pairePop[indice][1].gene[j];
    gene[10] = '\0';
}

// Applique un crossover sur notre population
void crossover(population *initPop, int taillePop) {
    population pairePop[taillePop/2][2];
    population newPop[taillePop/2];
    // Détermine 5 paires d'individus
    paire(initPop, pairePop, taillePop);
    // Initialise 5 nouveaux individus en fonction des paires précédentes
    for (int i = 0; i < taillePop/2; i++)
        create(newPop[i].gene, pairePop, i, taillePop);
    int index = 0;
    // Remplace les individus avec le score le plus faible par les nouveaux individus
    for (int k = 0; k < taillePop/2; k++) {
        index = 0;
        for (int i = 1; i < taillePop; i++)
            if (initPop[index].fitness > initPop[i].fitness)
                index = i;
        strcpy(initPop[index].gene, newPop[k].gene);
    }
}

// Applique une mutation sur la nouvelle population
void mutation(population *initPop, chaine dic, int taillePop) {
    for (int i = 0; i < taillePop; i++) {
        // Une mutation à 10% de chance de s'appliquer sur un des gènes
        if (valeurIntervalleZeroUn() <= 0.1) {
            // Choisis un gène et un mot du dictionnaire aléatoirement
            int index = valeurIntervalleZeroUn()*10;
            int indice = valeurIntervalleZeroUn()*T_DIC;
            initPop[i].gene[index] = dic[indice];
        }
    }
}

int main(void) {
    // Déclaration des variables nécessaires
    int generation = 0, indice = 0, taillePop = 0;
    bool found = false;
    chaine dic = "abcdefghijklmnopqrstuvwxyz ";
    mot answer = {0};

    // Demande le nombre d'individus
    printf("Saisir la taille de la population : ");
    scanf("%d", &taillePop);

    // Demande une chaîne de 10 caractères
    printf("Saisir un chaîne de 10 caractères : ");
    scanf("%9s", answer);
    answer[T_MOT] = '\0';

    // Remplis la population en fonction du dictionnaire précédent
    population initPop[taillePop];
    for (int i = 0; i < taillePop; i ++) {
        for (int j = 0; j < T_MOT; j ++) {
            indice = valeurIntervalleZeroUn()*T_DIC;
            initPop[i].gene[j] = dic[indice];
        }
        initPop[i].gene[T_MOT] = '\0';
        initPop[i].fitness = 0;
    }

    // Calcul le score des individus
    fitness(initPop, answer, &found, taillePop);
    // Affiche la population de départ
    afficheGeneration(initPop, generation, taillePop);
    while (!found) {
        // Ajoute une pause pour voir l'évolution de notre population
        usleep(50000);
        // Augmente la génération actuelle
        generation += 1;
        // Processus d'évolution (voir les commentaires des fonctions correspondantes)
        crossover(initPop, taillePop);
        mutation(initPop, dic, taillePop);
        fitness(initPop, answer, &found, taillePop);
        afficheGeneration(initPop, generation, taillePop);
    }
    // Quand une ressemble à 90% est trouvée, affiche ce message
    printf("Il y a une ressemblance à 90%%.\n");
}