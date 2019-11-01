#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "../lib/GfxLib.h"
#include "../lib/ESLib.h"
#include "ga.h"

// Détermine la distance de chaque individu avec l'arrivée
void fitness(individu tabIndividu[NB_INDIVIDUS], zone arrivee) {
    // Remets à 0 le score des individus
    for (int i = 0; i < NB_INDIVIDUS; i++)
        tabIndividu[i].fitness = 0;
    // Calcul la distance des individus avec l'arrivée
    for (int i = 0; i < NB_INDIVIDUS; i++)
        tabIndividu[i].fitness = sqrt(pow(arrivee.x - tabIndividu[i].x[NB_INDIVIDUS-1], 2) + pow(arrivee.y - tabIndividu[i].y[NB_INDIVIDUS-1], 2));
}

// Affiche, dans le terminal, la population actuelle
void afficheGeneration(individu tabIndividu[NB_INDIVIDUS], int generation) {
    printf("Génération: %d\n", generation);
    for (int i = 0; i < NB_INDIVIDUS; i++) {
        printf("Individu n°%d: ", i+1);
        printf("Fitness: %f\n", tabIndividu[i].fitness);
    }
}

// Tirage au sort sur une roue biaisée
void frise(individu tabIndividu[NB_INDIVIDUS], individu *paireIndividu, float total) {
    // Affectation de la valeur aléatoire
    float alea = valeurIntervalleZeroUn()*total, old = 0;
    for (int j = 0; j < NB_INDIVIDUS; j++) {
        // Copie la distance de l'individu actuel
        old += 1/tabIndividu[j].fitness;
        if (alea <= old) {
            // Dans le cas où la valeur aléatoire est inférieure ou égale au score actuel, on garde ces angles
            for (int i = 0; i < NB_INDIVIDUS-1; i++)
                paireIndividu->angle[i] = tabIndividu[j].angle[i];
            break;
        } 
    }
}

// Met en place des paires d'individus pour le crossover
void paire(individu tabIndividu[NB_INDIVIDUS], individu paireIndividu[NB_INDIVIDUS-1][2]) {
    float total = 0;
    // Détermine le score total mais inversé pour que les villes avec la distance la plus courte
    // est plus de chance d'être sélectionnées
    for (int i = 0; i < NB_INDIVIDUS; i++)
        total += 1/tabIndividu[i].fitness;
    for (int i = 0; i < NB_INDIVIDUS-1; i++) {
        frise(tabIndividu, &(paireIndividu[i][0]), total);
        frise(tabIndividu, &(paireIndividu[i][1]), total);
    }
}

// Définit notre nouvel individu
void create(individu newIndividu[NB_INDIVIDUS-1], individu paireIndividu[NB_INDIVIDUS-1][2], int indice) {
    // Détermine la valeur aléatoire qui permettra de sélection un certain pourcentage de chaque individu
    int prop = valeurIntervalleZeroUn()*(NB_INDIVIDUS-1);
    // Remplit notre nouvel individu en fonction de la valeur aléatoire précédente (avec la paire 1)
    for (int i = 0; i < prop; i++)
        newIndividu[indice].angle[i] = paireIndividu[indice][0].angle[i];
    // Remplit le reste de notre nouvel individu avec la paire 2
    for (int j = prop; j < NB_INDIVIDUS-1; j++)
        newIndividu[indice].angle[j] = paireIndividu[indice][1].angle[j];
}

// Applique un crossover sur notre population
void crossover(individu tabIndividu[NB_INDIVIDUS]) {
    individu paireIndividu[NB_INDIVIDUS-1][2];
    individu newIndividu[NB_INDIVIDUS-1];
    // Détermine NB_INDIVIDUS-1 paires d'individus
    paire(tabIndividu, paireIndividu);
    // Initialise NB_INDIVIDUS-1 nouveaux individus en fonction des paires précédentes
    for (int i = 0; i < NB_INDIVIDUS-1; i++)
        create(newIndividu, paireIndividu, i);
    // Détermine l'individu avec la distance la plus courte
    int top = 0;
    for (int i = 1; i < NB_INDIVIDUS; i++)
        if (tabIndividu[top].fitness > tabIndividu[i].fitness)
            top = i;
    // Remplace les individus par les nouveaux (sauf celui avec la plus petite distance)
    int index = 0;
    for (int k = 0; k < NB_INDIVIDUS-1; k++) {
        if (index == top)
            index += 1;
        tabIndividu[index] = newIndividu[k];
        computeCoord(&(tabIndividu[index]));
        index += 1;
    }
}

// Détermine les coordonnées des points de chaque individu
void computeCoord(individu *tabIndividu) {
	double angle = 0;
	int longueur = 155;
	for (int i = 1; i < NB_CHEMINS; i++) {
		// Détermine l'angle pour le prochain trait
		angle = 2.*M_PI*tabIndividu->angle[i-1];
		// Détermine les coordonnées du prochain point
		tabIndividu->x[i] = tabIndividu->x[i-1] + longueur*cos(angle);
		tabIndividu->y[i] = tabIndividu->y[i-1] + longueur*sin(angle);
	}
}

// Applique une mutation sur la nouvelle population
void mutation(individu tabIndividu[NB_INDIVIDUS]) {
    int index = 0;
    for (int i = 0; i < NB_INDIVIDUS; i++) {
        // Une mutation à 5% de chance de s'appliquer
        if (valeurIntervalleZeroUn() <= 0.05) {
            // Choisis un angle aléatoire
            index = valeurIntervalleZeroUn()*(NB_CHEMINS-1);
            tabIndividu[i].angle[index] = valeurIntervalleZeroUn();
            computeCoord(&(tabIndividu[i]));
        }
    }
}