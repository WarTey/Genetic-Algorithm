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

individu selectionIndividus(individu tabIndividu[NB_INDIVIDUS]) {
	// Sélection de deux individus par tournoi
	float fAlea = valeurIntervalleZeroUn()*NB_INDIVIDUS, sAlea = valeurIntervalleZeroUn()*NB_INDIVIDUS;
	individu fIndividu = tabIndividu[fAlea];
	individu sIndividu = tabIndividu[sAlea];
	// Garde l'individu avec le score le plus élevé
	return fIndividu.total > sIndividu.total ? fIndividu : sIndividu;
}

// Crée des paires d'individus
void paire(individu tabIndividu[NB_INDIVIDUS], individu paireIndividu[NB_INDIVIDUS-1][2]) {
	for (int i = 0; i < NB_INDIVIDUS-1; i++) {
		individu fIndividu = selectionIndividus(tabIndividu);
		individu sIndividu = selectionIndividus(tabIndividu);
		// Recherche un autre individu tant qu'il est similaire au premier
		while (compareIndividu(fIndividu, sIndividu))
			sIndividu = selectionIndividus(tabIndividu);
		paireIndividu[i][0] = fIndividu;
		paireIndividu[i][1] = sIndividu;
	}
}

void crossover(float *newIndividu, float fParam, float sParam, int cMin, int cMax) {
	// Crossover du gène
	int min = fParam < sParam ? fParam : sParam;
	int max = fParam > sParam ? fParam : sParam;
	*newIndividu = valeurIntervalleZeroUn()*(max-min)+min;
	// Mutation
	if (valeurAleatoire() < 0.1) {
		float indice = (valeurAleatoire() < 0.5 ? -1 : 1)*valeurIntervalleZeroUn()*(cMax-cMin)*0.5;
		if ((*newIndividu == cMin && indice < 0) || (*newIndividu == cMax && indice > 0))
			indice = -indice;
		*newIndividu += indice;
		if (*newIndividu < cMin)
			*newIndividu = cMin;
		if (*newIndividu > cMax)
			*newIndividu = cMax;
	}
}

void transformation(individu tabIndividu[NB_INDIVIDUS]) {
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
		// Applique un crossover sur chaque gène
		newIndividu[i].panier = 0;
		int min = paireIndividu[i][0].total < paireIndividu[i][1].total ? paireIndividu[i][0].total : paireIndividu[i][1].total;
		int max = paireIndividu[i][0].total > paireIndividu[i][1].total ? paireIndividu[i][0].total : paireIndividu[i][1].total;
		newIndividu[i].total = valeurIntervalleZeroUn()*(max-min)+min;
		crossover(&(newIndividu[i].x), paireIndividu[i][0].x, paireIndividu[i][1].x, -29, largeurFenetre()+29);
		crossover(&(newIndividu[i].y), paireIndividu[i][0].y, paireIndividu[i][1].y, hauteurFenetre()/16-29, hauteurFenetre()+29);
		crossover(&(newIndividu[i].xProb), paireIndividu[i][0].xProb, paireIndividu[i][1].xProb, 1, 6);
		crossover(&(newIndividu[i].yProb), paireIndividu[i][0].yProb, paireIndividu[i][1].yProb, 1, 6);
		crossover(&(newIndividu[i].xVitesse), paireIndividu[i][0].xVitesse, paireIndividu[i][1].xVitesse, 1, 5);
		crossover(&(newIndividu[i].yVitesse), paireIndividu[i][0].yVitesse, paireIndividu[i][1].yVitesse, 1, 5);
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
	// Remplace nos individus les plus faibles par les nouveaux
	for (int i = 0; i < NB_INDIVIDUS/2; i++)
		tabIndividu[i+NB_INDIVIDUS/2] = newIndividu[i];
	for (int i = 0; i < NB_INDIVIDUS; i++)
		tabIndividu[i].total = 0;
}