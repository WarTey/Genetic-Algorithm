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
void paire(individu tabIndividu[10], individu paireIndividu[5][2]) {
	// Accumule tous les paniers
	int totalPanier = 0, alea = 0, lastPanier = 0;
	for (int i = 0; i < 10; i++)
		totalPanier += tabIndividu[i].total;
	// On choisit nos paires sur une roue biaisée
	for (int k = 0; k < 5; k++) {
		for (int i = 0; i < 2; i++) {
			lastPanier = 0;
			alea = valeurIntervalleZeroUn()*totalPanier;
			for (int j = 0; j < 10; j++) {
				lastPanier += tabIndividu[j].total;
				if (alea <= lastPanier) {
					paireIndividu[k][i] = tabIndividu[j];
					break;
				}
			}
		}
	}
}

void crossover(float *newIndividu, float fParam, float sParam, int max) {
	// Mutation
	if (valeurAleatoire() < 0.1)
		*newIndividu = valeurIntervalleZeroUn()*max+1;
	// Crossover
	else
		*newIndividu = valeurIntervalleZeroUn() > 0.5 ? fParam : sParam;
}

void transformation(individu tabIndividu[10]) {
	// On ajoute les paniers au score total de chaque individu
	for (int i = 0; i < 10; i++) {
		tabIndividu[i].total += tabIndividu[i].panier;
		tabIndividu[i].panier = 0;
	}
	// On choisit 5 paires d'individus qui vont être 'crossover'
	individu paireIndividu[5][2];
	paire(tabIndividu, paireIndividu);
	// On crée 5 nouveaux individus
	individu newIndividu[5];
	for (int i = 0; i < 5; i++) {
		// Applique le crossover sur chaque gène
		newIndividu[i].panier = 0;
		newIndividu[i].last = -1;
		newIndividu[i].total = (paireIndividu[i][0].total+paireIndividu[i][1].total)/2;
		newIndividu[i].x = (paireIndividu[i][0].x+paireIndividu[i][1].x)/2;
		newIndividu[i].y = (paireIndividu[i][0].y+paireIndividu[i][1].y)/2;
		crossover(&(newIndividu[i].xProb), paireIndividu[i][0].xProb, paireIndividu[i][1].xProb, 5);
		crossover(&(newIndividu[i].yProb), paireIndividu[i][0].yProb, paireIndividu[i][1].yProb, 5);
		crossover(&(newIndividu[i].xVitesse), paireIndividu[i][0].xVitesse, paireIndividu[i][1].xVitesse, 4);
		crossover(&(newIndividu[i].yVitesse), paireIndividu[i][0].yVitesse, paireIndividu[i][1].yVitesse, 4);
	}
	// Tri nos individus
	for (int i = 0; i < 9; i++) {
		for (int j = i+1; j < 10; j++) {
			if (tabIndividu[i].total < tabIndividu[j].total) {
				individu temp = tabIndividu[i];
				tabIndividu[i] = tabIndividu[j];
				tabIndividu[j] = temp;
			}
		}
	}
	// Remplace nos individus les plus faibles par les nouveaux
	for (int i = 0; i < 5; i++)
		tabIndividu[i+5] = newIndividu[i];
	for (int i = 0; i < 10; i++)
		tabIndividu[i].total = 0;
}