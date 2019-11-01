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

// Affiche le chemin généré pour chaque individu
void afficheIndividu(individu tabIndividu[NB_INDIVIDUS]) {
	for (int i = 0; i < NB_INDIVIDUS; i++) {
		for (int j = 1; j < NB_CHEMINS; j++) {
			// Affiche la ligne d'un bout de chemin
			couleurCourante(0, 0, 255);
			ligne(tabIndividu[i].x[j-1], tabIndividu[i].y[j-1], tabIndividu[i].x[j], tabIndividu[i].y[j]);
			// Affiche le point liant chaque bout de chemin
			couleurCourante(255, 0, 0);
			cercle(tabIndividu[i].x[j], tabIndividu[i].y[j], 2);
		}
	}
}

void generationIndividu(individu tabIndividu[NB_INDIVIDUS]) {
	// Génération des individus
	for (int i = 0; i < NB_INDIVIDUS; i++) {
		// Score de chaque individu initialement à 0
		tabIndividu[i].fitness = 0;
		// Définit le point de départ à (0, 0)
		for (int j = 0; j < NB_INDIVIDUS; j++) {
			tabIndividu[i].x[j] = 0;
			tabIndividu[i].y[j] = 0;
		}
		// Génération des angles
		for (int j = 0; j < NB_CHEMINS-1; j++)
			tabIndividu[i].angle[j] = valeurIntervalleZeroUn();
		computeCoord(&(tabIndividu[i]));
	}
}

bool checkEnd(individu tabIndividu[NB_INDIVIDUS], zone arrivee) {
	// Vérifie si un des chemins a attend la zone d'arrivée
	for (int i = 0; i < NB_INDIVIDUS; i++)
		if (tabIndividu[i].x[NB_CHEMINS-1] > arrivee.x-arrivee.taille && tabIndividu[i].x[NB_CHEMINS-1] < arrivee.x+arrivee.taille && tabIndividu[i].y[NB_CHEMINS-1] > arrivee.y-arrivee.taille && tabIndividu[i].y[NB_CHEMINS-1] < arrivee.y+arrivee.taille)
			return false;
	return true;
}

int main(int argc, char **argv) {
	initialiseGfx(argc, argv);
	prepareFenetreGraphique("Travelling salesman problem", LargeurFenetre, HauteurFenetre);
	lanceBoucleEvenements();
	return 0;
}

void gestionEvenement(EvenementGfx evenement) {
	static bool pleinEcran = false;
	static individu tabIndividu[NB_INDIVIDUS];
	static int generation = 0;
	static char afficheEvolution[100];
	static zone depart;
	static zone arrivee;
	
	switch (evenement) {
		case Initialisation:
			// Génère le point de départ et d'arrivée
			depart.x = 0;
			depart.y = hauteurFenetre()/16;
			depart.taille = 35;
			arrivee.x = largeurFenetre();
			arrivee.y = hauteurFenetre();
			arrivee.taille = 35;
			// Génération des individus
			generationIndividu(tabIndividu);
			// Calcul la distance des individus par rapport à l'arrivée
			fitness(tabIndividu, arrivee);
			// Affiche la population de départ
			afficheGeneration(tabIndividu, generation);
			demandeTemporisation(20);
			break;
		
		case Temporisation:
			if (checkEnd(tabIndividu, arrivee)) {
				// Augmente la génération actuelle
				generation += 1;
				// Processus d'évolution (voir les commentaires des fonctions correspondantes)
				crossover(tabIndividu);
				mutation(tabIndividu);
				fitness(tabIndividu, arrivee);
				afficheGeneration(tabIndividu, generation);
			}
			rafraichisFenetre();
			break;
			
		case Affichage:
			// Départ écran blanc
			effaceFenetre (255, 255, 255);
			// Affichage départ
			couleurCourante(0, 255, 0);
			cercle(0, hauteurFenetre()/16, 35);
			// Affichage arrivée
			couleurCourante(255, 0, 0);
			cercle(largeurFenetre(), hauteurFenetre(), 35);
			// Affichage des individus
			afficheIndividu(tabIndividu);
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
			// Génération actuelle
			sprintf(afficheEvolution, "Generation n.%d", generation);
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
