#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "lib/GfxLib.h"
#include "lib/ESLib.h"
#include "ga/ga.h"

#define LargeurFenetre 1200
#define HauteurFenetre 720

void cercle(float centreX, float centreY, float rayon) {
	const int Pas = 180;
	const double PasAngulaire = 2.*M_PI/Pas;
	int index;
	for (index = 0; index < Pas; ++index) {
		const double angle = 2.*M_PI*index/Pas;
		triangle(centreX, centreY, centreX+rayon*cos(angle), centreY+rayon*sin(angle), centreX+rayon*cos(angle+PasAngulaire), centreY+rayon*sin(angle+PasAngulaire));
	}
}

void generationIndividu(individu tabIndividu[10]) {
	// Génération des individus
	int ville = 0;
	// Définit la distance de chaque individu à 0
	for (int i = 0; i < 10; i++) {
		tabIndividu[i].fitness = 0;
		// Répartition des chemins
		for (int j = 0; j < 10; j++) {
			ville = valeurIntervalleZeroUn()*10;
			for (int k = 0; k < j; k++) {
				if (tabIndividu[i].chemin[k] == ville) {
					ville = valeurIntervalleZeroUn()*10;
					k = -1;
				}
			}
			tabIndividu[i].chemin[j] = ville;
		}
	}
}

void checkVille(ville tabVille[10]) {
	// Repositionne une ville en cas de mauvais placement
	bool indice = false;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (i != j) {
				// Vérifie qu'une ville ne soit pas trop proche d'une autre
				while (tabVille[i].x <= 5 || tabVille[i].y <= hauteurFenetre()/16+5 || (tabVille[i].x >= tabVille[j].x-100 && tabVille[i].x <= tabVille[j].x+100 && tabVille[i].y >= tabVille[j].y-100 && tabVille[i].y <= tabVille[j].y+100)) {
					indice = true;
					tabVille[i].x = valeurAleatoire()*largeurFenetre();
					tabVille[i].y = valeurAleatoire()*hauteurFenetre();
				}
				if (indice) {
					indice = false;
					i = 0;
					j = 0;
				}
			}
		}
	}
}

void generationVille(ville tabVille[10]) {
	// Génération des villes
	for (int i = 0; i < 10; i++) {
		tabVille[i].x = 0;
		tabVille[i].y = 0;
		// Vérifie qu'une ville ne soit pas trop proche d'un des bords de l'écran
		while (tabVille[i].x <= 5 || tabVille[i].y <= hauteurFenetre()/16+5 || (tabVille[i].x >= largeurFenetre()/2-150 && tabVille[i].x <= largeurFenetre()/2+150 && tabVille[i].y >= hauteurFenetre()/2+hauteurFenetre()/16-150 && tabVille[i].y <= hauteurFenetre()/2+hauteurFenetre()/16+150)) {
			tabVille[i].x = valeurAleatoire()*largeurFenetre();
			tabVille[i].y = valeurAleatoire()*hauteurFenetre();
		}
	}
	checkVille(tabVille);
}

void afficheVille(ville tabVille[10]) {
	// Affiche les villes
	couleurCourante(0, 125, 0);
	for (int i = 0; i < 10; i++)
		rectangle(tabVille[i].x-5, tabVille[i].y-5, tabVille[i].x+5, tabVille[i].y+5);
}

void afficheChemin(individu tabIndividu[10], ville tabVille[10], float *distance) {
	// Affiche le meilleur chemin (celui avec la distance la plus courte)
	int index = 0;
	// Cherche l'individu avec la distance la plus courte
	for (int i = 1; i < 10; i++)
		if (tabIndividu[index].fitness > tabIndividu[i].fitness)
			index = i;
	couleurCourante(125, 0, 0);
	epaisseurDeTrait(2);
	// Affiche l'individu en question
	for (int i = 1; i < 10; i++)
		ligne(tabVille[tabIndividu[index].chemin[i-1]].x, tabVille[tabIndividu[index].chemin[i-1]].y, tabVille[tabIndividu[index].chemin[i]].x, tabVille[tabIndividu[index].chemin[i]].y);
	ligne(tabVille[tabIndividu[index].chemin[0]].x, tabVille[tabIndividu[index].chemin[0]].y, tabVille[tabIndividu[index].chemin[9]].x, tabVille[tabIndividu[index].chemin[9]].y);
	// Enregistre sa distance
	*distance = tabIndividu[index].fitness;
}

int main(int argc, char **argv) {
	initialiseGfx(argc, argv);
	prepareFenetreGraphique("Travelling salesman problem", LargeurFenetre, HauteurFenetre);
	lanceBoucleEvenements();
	return 0;
}

void gestionEvenement(EvenementGfx evenement) {
	static bool pleinEcran = false;
	static individu tabIndividu[10];
	static ville tabVille[10];
	static int evolution = 0;
	static float distance = 0;
	static char afficheDistance[100], afficheEvolution[100];
	
	switch (evenement) {
		case Initialisation:
			// Génération des villes
			generationVille(tabVille);
			// Génération des individus
			generationIndividu(tabIndividu);
			// Calcul la distance des individus
			fitness(tabIndividu, tabVille);
			// Affiche la population de départ
			afficheGeneration(tabIndividu, evolution);
			demandeTemporisation(20);
			break;
		
		case Temporisation:
			if (evolution < 500) {
				// Augmente la génération actuelle
				evolution += 1;
				// Processus d'évolution (voir les commentaires des fonctions correspondantes)
				crossover(tabIndividu);
				mutation(tabIndividu);
				fitness(tabIndividu, tabVille);
				afficheGeneration(tabIndividu, evolution);
			}
			rafraichisFenetre();
			break;
			
		case Affichage:
			// Départ écran blanc
			effaceFenetre (255, 255, 255);
			// Affichage des villes
			afficheVille(tabVille);
			// Affichage du meilleur chemin
			afficheChemin(tabIndividu, tabVille, &distance);
			// Traçage d'un rectangle blanc
			couleurCourante(255, 255, 255);
			rectangle(0, 0, largeurFenetre(), hauteurFenetre()/16);
			// Traçage d'une ligne en bas de l'écran
			couleurCourante(0, 0, 0);
			epaisseurDeTrait(2);
			ligne(0, hauteurFenetre()/16, largeurFenetre(), hauteurFenetre()/16);
			// Affiche d'une chaîne de caractères à l'écran
			couleurCourante(0, 0, 0);
			epaisseurDeTrait(2);
			// Meilleure distance (la plus courte)
			sprintf(afficheDistance, "Distance: %f", distance);
			afficheChaine(afficheDistance, hauteurFenetre()/32, largeurFenetre()/2, hauteurFenetre()/64);
			// Génération actuelle
			sprintf(afficheEvolution, "Evolution n.%d", evolution);
			afficheChaine(afficheEvolution, hauteurFenetre()/32, largeurFenetre()/128, hauteurFenetre()/64);
			break;
			
		case Clavier:
			switch (caractereClavier()) {
				case 'Q':
				case 'q':
					termineBoucleEvenements();
					break;

				case 'F':
				case 'f':
					pleinEcran = !pleinEcran;
					if (pleinEcran)
						modePleinEcran();
					else
						redimensionneFenetre(LargeurFenetre, HauteurFenetre);
					break;

				case 'C':
				case 'c':
					demandeTemporisation(20);
					break;

				case 'P':
				case 'p':
					demandeTemporisation(-1);
					break;
			}
			break;

		case ClavierSpecial:
			break;
			
		case BoutonSouris:
			break;
		
		case Souris:
			break;
		
		case Inactivite:
			break;
		
		case Redimensionnement:
			break;
	}
}
