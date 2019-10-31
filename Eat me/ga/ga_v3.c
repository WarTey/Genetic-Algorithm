#include <stdbool.h>
#include "../lib/GfxLib.h"
#include "../lib/ESLib.h"
#include "ga.h"

// Compare deux individus (structure)
bool compareIndividu(individu fIndividu, individu sIndividu) {
	if (fIndividu.xVitesse == sIndividu.xVitesse && fIndividu.yVitesse == sIndividu.yVitesse
			&& fIndividu.xProb == sIndividu.xProb && fIndividu.yProb == sIndividu.yProb
			&& fIndividu.x == sIndividu.x && fIndividu.y == sIndividu.y
			&& fIndividu.panier == sIndividu.panier && fIndividu.total == sIndividu.total)
		return true;
	return false;
}

// Crée des paires d'individus
void paire(individu tabIndividu[NB_INDIVIDUS], individu paireIndividu[NB_INDIVIDUS-1][2]) {
	// Accumule tous les paniers
	int totalPanier = 0, lastPanier = 0;
	float alea = 0;
	for (int i = 0; i < NB_INDIVIDUS; i++)
		totalPanier += tabIndividu[i].total;
	// On choisit nos paires sur une roue biaisée
	for (int k = 0; k < NB_INDIVIDUS-1; k++) {
		for (int i = 0; i < 2; i++) {
			lastPanier = 0;
			// Choisit une valeur aléatoire
			alea = valeurIntervalleZeroUn()*totalPanier;
			for (int j = 0; j < NB_INDIVIDUS; j++) {
				// Ajoute le score de l'individu actuel
				lastPanier += tabIndividu[j].total;
				// Si la valeur générée est sur l'intervalle du score de l'individu, alors l'enregistre
				if (alea <= lastPanier) {
					paireIndividu[k][i] = tabIndividu[j];
					break;
				}
			}
		}
	}
}

void crossover(float *newIndividu, float fParam, float sParam, int max, bool mutation) {
	// Mutation (1% de chance de se produire)
	if (mutation && valeurAleatoire() < 0.01) {
		mutation = false;
		*newIndividu = valeurIntervalleZeroUn()*max+1;
	// Crossover
	} else
		*newIndividu = valeurIntervalleZeroUn() > 0.5 ? fParam : sParam;
}

void transformation(individu tabIndividu[NB_INDIVIDUS]) {
	bool mutation = false;
	// On ajoute les paniers au score total de chaque individu
	for (int i = 0; i < NB_INDIVIDUS; i++) {
		tabIndividu[i].total += tabIndividu[i].panier;
		tabIndividu[i].panier = 0;
	}
	// On choisit NB_INDIVIDUS-1 paires d'individus qui vont être 'crossover'
	individu paireIndividu[NB_INDIVIDUS-1][2];
	paire(tabIndividu, paireIndividu);
	// On crée NB_INDIVIDUS-1 nouveaux individus
	individu newIndividu[NB_INDIVIDUS-1];
	for (int i = 0; i < NB_INDIVIDUS-1; i++) {
		mutation = true;
		// Applique le crossover sur chaque gène
		newIndividu[i].panier = 0;
		newIndividu[i].last = -1;
		newIndividu[i].total = (paireIndividu[i][0].total+paireIndividu[i][1].total)/2;
		newIndividu[i].x = (paireIndividu[i][0].x+paireIndividu[i][1].x)/2;
		newIndividu[i].y = (paireIndividu[i][0].y+paireIndividu[i][1].y)/2;
		crossover(&(newIndividu[i].xProb), paireIndividu[i][0].xProb, paireIndividu[i][1].xProb, 5, mutation);
		crossover(&(newIndividu[i].yProb), paireIndividu[i][0].yProb, paireIndividu[i][1].yProb, 5, mutation);
		crossover(&(newIndividu[i].xVitesse), paireIndividu[i][0].xVitesse, paireIndividu[i][1].xVitesse, 4, mutation);
		crossover(&(newIndividu[i].yVitesse), paireIndividu[i][0].yVitesse, paireIndividu[i][1].yVitesse, 4, mutation);
	}
	// Tri nos individus
	for (int i = 0; i < NB_INDIVIDUS-1; i++) {
		for (int j = i+1; j < NB_INDIVIDUS; j++) {
			if (tabIndividu[i].total < tabIndividu[j].total) {
				individu temp = tabIndividu[i];
				tabIndividu[i] = tabIndividu[j];
				tabIndividu[j] = temp;
			}
		}
	}
	// Remplace nos individus les plus faibles par les nouveaux (pour garder le meilleur)
	for (int i = 1; i < NB_INDIVIDUS; i++)
		tabIndividu[i] = newIndividu[i-1];
	for (int i = 0; i < NB_INDIVIDUS; i++)
		tabIndividu[i].total = 0;
}