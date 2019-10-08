#include <stdbool.h>
#include "../lib/GfxLib.h"
#include "../lib/ESLib.h"
#include "ga.h"

bool compareIndividu(individu fIndividu, individu sIndividu) {
	if (fIndividu.xVitesse == sIndividu.xVitesse && fIndividu.yVitesse == sIndividu.yVitesse
			&& fIndividu.xProb == sIndividu.xProb && fIndividu.yProb == sIndividu.yProb
			&& fIndividu.x == sIndividu.x && fIndividu.y == sIndividu.y
			&& fIndividu.panier == sIndividu.panier && fIndividu.total == sIndividu.total)
		return true;
	return false;
}

individu selectionIndividus(individu tabIndividu[10]) {
	// Sélection de deux individus
	int fAlea = valeurIntervalleZeroUn()*10, sAlea = valeurIntervalleZeroUn()*10;
	individu fIndividu = tabIndividu[fAlea];
	individu sIndividu = tabIndividu[sAlea];
	return fIndividu.total > sIndividu.total ? fIndividu : sIndividu;
}

void paire(individu tabIndividu[10], individu paireIndividu[5][2]) {
	for (int i = 0; i < 5; i++) {
		individu fIndividu = selectionIndividus(tabIndividu);
		individu sIndividu = selectionIndividus(tabIndividu);
		while (compareIndividu(fIndividu, sIndividu))
			sIndividu = selectionIndividus(tabIndividu);
		paireIndividu[i][0] = fIndividu;
		paireIndividu[i][1] = sIndividu;
	}
}

void crossover(float *newIndividu, float fParam, float sParam, int cMin, int cMax) {
	// Crossover de la caractéristique
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
	for (int i = 0; i < 9; i++) {
		for (int j = i+1; j < 10; j++) {
			if (tabIndividu[i].total < tabIndividu[j].total) {
				individu temp = tabIndividu[i];
				tabIndividu[i] = tabIndividu[j];
				tabIndividu[j] = temp;
			}
		}
	}
	for (int i = 0; i < 5; i++)
		tabIndividu[i+5] = newIndividu[i];
	for (int i = 0; i < 10; i++)
		tabIndividu[i].total = 0;
}