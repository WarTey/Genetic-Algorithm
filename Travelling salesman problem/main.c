#include <stdio.h>
#include <math.h>
#include "lib/GfxLib.h"
#include "lib/ESLib.h"
#include "ga/ga.h"

#define LargeurFenetre 1200
#define HauteurFenetre 720

typedef struct ville {
	float x, y;
} ville;

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
	// Vérifie que les villes ne soient pas trop proches
	bool indice = false;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (i != j) {
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

int main(int argc, char **argv) {
	initialiseGfx(argc, argv);
	prepareFenetreGraphique("Eat Me", LargeurFenetre, HauteurFenetre);
	lanceBoucleEvenements();
	return 0;
}

void gestionEvenement(EvenementGfx evenement) {
	static bool pleinEcran = false;
	static individu tabIndividu[10];
	static ville tabVille[10];
	static int distance = 0, evolution = 0;
	static char afficheDistance[100], afficheEvolution[100];
	
	switch (evenement) {
		case Initialisation:
			generationVille(tabVille);
			generationIndividu(tabIndividu);
			demandeTemporisation(20);
			break;
		
		case Temporisation:
			rafraichisFenetre();
			break;
			
		case Affichage:
			// Départ écran blanc
			effaceFenetre (255, 255, 255);
			// Affichage des villes
			afficheVille(tabVille);
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
			sprintf(afficheDistance, "Distance: %d", distance);
			afficheChaine(afficheDistance, hauteurFenetre()/32, largeurFenetre()/2, hauteurFenetre()/64);
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
					// Génération des villes
					generationVille(tabVille);
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
			// Génération des villes
			generationVille(tabVille);
			break;
	}
}
