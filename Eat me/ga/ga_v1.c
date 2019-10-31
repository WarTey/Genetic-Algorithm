#include "../lib/GfxLib.h"
#include "../lib/ESLib.h"
#include "ga.h"

void transformation(individu tabIndividu[NB_INDIVIDUS]) {
	// On ajoute les paniers au score total de chaque indivu
	for (int i = 0; i < NB_INDIVIDUS; i++) {
		tabIndividu[i].total += tabIndividu[i].panier;
		tabIndividu[i].panier = 0;
	}
	// Accumule tous les paniers
	int totalPanier = 0, lastPanier = 0;
	float alea = 0;
	for (int i = 0; i < NB_INDIVIDUS; i++)
		totalPanier += tabIndividu[i].total;
	// On crée nos nouveaux individus
	individu newIndividu[NB_INDIVIDUS];
	for (int i = 0; i < NB_INDIVIDUS; i++) {
		lastPanier = 0;
		alea = valeurIntervalleZeroUn()*totalPanier;
		// On utilise le principe de la roue biaisée
		for (int j = 0; j < NB_INDIVIDUS; j++) {
			lastPanier += tabIndividu[j].total;
			if (alea <= lastPanier) {
				newIndividu[i] = tabIndividu[j];
				break;
			}
		}
	}
	// On réinject les individus avec une possible mutation
	for (int i = 0; i < NB_INDIVIDUS; i++) {
		tabIndividu[i] = newIndividu[i];
		tabIndividu[i].total = 0;
		if (valeurIntervalleZeroUn() >= 0.90) {
			alea = valeurIntervalleZeroUn()*4;
			switch(alea) {
				case 0:
					tabIndividu[i].xProb = valeurIntervalleZeroUn()*5+1;
					break;

				case 1:
					tabIndividu[i].yProb = valeurIntervalleZeroUn()*5+1;
					break;

				case 2:
					tabIndividu[i].xVitesse = valeurIntervalleZeroUn()*4+1;
					break;

				case 3:
					tabIndividu[i].yVitesse = valeurIntervalleZeroUn()*4+1;
					break;
			}
		}
	}
}