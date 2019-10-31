#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "lib/ESLib.h"
#include "ga/ga.h"

int main(void) {
    // Déclaration des variables nécessaires
    int generation = 0, indice = 0, taillePop = 0;
    bool found = false;
    // Dictionnaire contenant toutes les lettres à utiliser
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
        // Ajoute une courte pause pour voir l'évolution de notre population
        usleep(50000);
        // Augmente la génération actuelle
        generation += 1;
        // Processus d'évolution (voir les commentaires des fonctions correspondantes)
        crossover(initPop, taillePop);
        mutation(initPop, dic, taillePop);
        fitness(initPop, answer, &found, taillePop);
        afficheGeneration(initPop, generation, taillePop);
    }
    // Quand une ressemble à 90% est trouvée, affiche ce message et fin de la boucle
    printf("Il y a une ressemblance à 90%%.\n");
}