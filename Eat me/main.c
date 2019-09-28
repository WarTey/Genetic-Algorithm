#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lib/GfxLib.h"
#include "lib/BmpLib.h"
#include "lib/ESLib.h"

#define LargeurFenetre 1200
#define HauteurFenetre 720

typedef struct individu {
	int xVitesse, yVitesse;
	float xProb, yProb;
	float x, y;
	int panier;
	int total;
} individu;

typedef struct nourriture {
	float x, y;
} nourriture;

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
	for (int i = 0; i < 10; i++) {
		tabIndividu[i].panier = 0;
		tabIndividu[i].total = 0;
		// Démarrage au centre de la maison
		tabIndividu[i].x = largeurFenetre()/2;
		tabIndividu[i].y = hauteurFenetre()/2+hauteurFenetre()/16;
		// Probalité de changer de direction (entre 1% et 6%)
		tabIndividu[i].xProb = valeurIntervalleZeroUn()*6+1;
		tabIndividu[i].yProb = valeurIntervalleZeroUn()*6+1;
		// Vitesse en abscisse (entre 1 et 5)
		tabIndividu[i].xVitesse = valeurIntervalleZeroUn()*4+1;
		// Vitesse en ordonnée (entre 1 et 5)
		tabIndividu[i].yVitesse = valeurIntervalleZeroUn()*4+1;
	}
}

void generationNourriture(nourriture tabNourriture[20]) {
	// Génération de la nourriture aléatoirement sur le terrain
	for (int i = 0; i < 20; i++) {
		tabNourriture[i].x = 0;
		tabNourriture[i].y = 0;
		while (tabNourriture[i].x <= 5 || tabNourriture[i].y <= hauteurFenetre()/16+5 || (tabNourriture[i].x >= largeurFenetre()/2-80 && tabNourriture[i].x <= largeurFenetre()/2+80 && tabNourriture[i].y >= hauteurFenetre()/2+hauteurFenetre()/16-80 && tabNourriture[i].y <= hauteurFenetre()/2+hauteurFenetre()/16+80)) {
			tabNourriture[i].x = valeurAleatoire()*largeurFenetre();
			tabNourriture[i].y = valeurAleatoire()*hauteurFenetre();
		}
	}
}

void afficheIndividu(individu tabIndividu[10]) {
	// Affiche les individus avec leur score
	char scoreIndividu[100];
	for (int i = 0; i < 10; i++) {
		couleurCourante(0, 0, 215);
		cercle(tabIndividu[i].x, tabIndividu[i].y, 15);
		couleurCourante(255, 255, 255);
		epaisseurDeTrait(2);
		sprintf(scoreIndividu, "%d", tabIndividu[i].total + tabIndividu[i].panier);
		afficheChaine(scoreIndividu, 10, tabIndividu[i].x-tailleChaine(scoreIndividu, 10), tabIndividu[i].y-5);
	}
}

void afficheNourriture(nourriture tabNourriture[20]) {
	// Affiche la nourriture
	couleurCourante(0, 125, 0);
	for (int i = 0; i < 20; i++)
		rectangle(tabNourriture[i].x-5, tabNourriture[i].y-5, tabNourriture[i].x+5, tabNourriture[i].y+5);
}

void deplacement(individu tabIndividu[10]) {
	// Déplacement aléatoire de l'individu selon ses caractéristiques
	for (int i = 0; i < 10; i++) {
		// Détermine si changement de direction
		tabIndividu[i].xVitesse = valeurIntervalleZeroUn()*100 <= tabIndividu[i].xProb ? -tabIndividu[i].xVitesse : tabIndividu[i].xVitesse;
		tabIndividu[i].yVitesse = valeurIntervalleZeroUn()*100 <= tabIndividu[i].yProb ? -tabIndividu[i].yVitesse : tabIndividu[i].yVitesse;
		// Déplacement de l'individu
		tabIndividu[i].x += tabIndividu[i].xVitesse;
		tabIndividu[i].y += tabIndividu[i].yVitesse;
		// Dans le cas où notre individu arrive en bord de zone
		if (tabIndividu[i].x > largeurFenetre()+30)
			tabIndividu[i].x = -30;
		else if (tabIndividu[i].x < -30)
			tabIndividu[i].x = largeurFenetre()+30;
		if (tabIndividu[i].y > hauteurFenetre()+30)
			tabIndividu[i].y = hauteurFenetre()/16-30;
		else if (tabIndividu[i].y < hauteurFenetre()/16-30)
			tabIndividu[i].y = hauteurFenetre()+30;
	}
}

void ramasse(individu tabIndividu[10], nourriture tabNourriture[20], int *score) {
	for (int i = 0; i < 10; i++) {
		// Un individu peut seulement porter 5 paniers
		if (tabIndividu[i].panier < 5) {
			for (int j = 0; j < 20; j++) {
				// Lorqu'un individu rentre en collision avec de la nourriture il l'a ramasse et une nouvelle nourriture est générée
				if (tabIndividu[i].x >= tabNourriture[j].x-20 && tabIndividu[i].x <= tabNourriture[j].x+20 && tabIndividu[i].y >= tabNourriture[j].y-20 && tabIndividu[i].y <= tabNourriture[j].y+20) {
					tabIndividu[i].panier += 1;
					*score += 1;
					tabNourriture[j].x = 0;
					tabNourriture[j].y = 0;
					while (tabNourriture[j].x <= 5 || tabNourriture[j].y <= hauteurFenetre()/16+5 || (tabNourriture[j].x >= largeurFenetre()/2-80 && tabNourriture[j].x <= largeurFenetre()/2+80 && tabNourriture[j].y >= hauteurFenetre()/2+hauteurFenetre()/16-80 && tabNourriture[j].y <= hauteurFenetre()/2+hauteurFenetre()/16+80)) {
						tabNourriture[j].x = valeurAleatoire()*largeurFenetre();
						tabNourriture[j].y = valeurAleatoire()*hauteurFenetre();
					}
				}
			}
		}
		// Lorqu'un individu rentre à la maison il dépose ses paniers
		if (tabIndividu[i].x >= largeurFenetre()/2-90 && tabIndividu[i].x <= largeurFenetre()/2+90 && tabIndividu[i].y >= hauteurFenetre()/2+hauteurFenetre()/16-90 && tabIndividu[i].y <= hauteurFenetre()/2+hauteurFenetre()/16+90) {
			tabIndividu[i].total += tabIndividu[i].panier;
			tabIndividu[i].panier = 0;
		}
	}
}

void acceleration(individu tabIndividu[10], bool accelerate) {
	// Augmente ou réduit la vitesse des individus
	for (int i = 0; i < 10; i++) {
		if (accelerate) {
			tabIndividu[i].xVitesse *= 2;
			tabIndividu[i].yVitesse *= 2;
		} else {
			tabIndividu[i].xVitesse /= 2;
			tabIndividu[i].yVitesse /= 2;
		}
	}
}

void selection(individu tabIndividu[10]) {
	// On ajoute les paniers au score total de chaque indivu
	for (int i = 0; i < 10; i++) {
		tabIndividu[i].total += tabIndividu[i].panier;
		tabIndividu[i].panier = 0;
	}
	// Accumule tout les paniers
	int totalPanier = 0, alea = 0, lastPanier = 0;
	for (int i = 0; i < 10; i++)
		totalPanier += tabIndividu[i].total;
	// On crée nos nouveaux individus
	individu newIndividu[10];
	for (int i = 0; i < 10; i++) {
		lastPanier = 0;
		alea = valeurIntervalleZeroUn()*totalPanier;
		for (int j = 0; j < 10; j++) {
			lastPanier += tabIndividu[j].total;
			if (alea <= lastPanier) {
				newIndividu[i] = tabIndividu[j];
				break;
			}
		}
	}
	// On réinject les individus avec une possible mutation
	for (int i = 0; i < 10; i++) {
		tabIndividu[i] = newIndividu[i];
		if (valeurIntervalleZeroUn() >= 0.85) {
			alea = valeurIntervalleZeroUn()*4;
			switch(alea) {
				case 0:
					tabIndividu[i].xProb = valeurIntervalleZeroUn()*6+1;
					break;

				case 1:
					tabIndividu[i].yProb = valeurIntervalleZeroUn()*6+1;
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

int main(int argc, char **argv) {
	initialiseGfx(argc, argv);
	prepareFenetreGraphique("Eat Me", LargeurFenetre, HauteurFenetre);
	lanceBoucleEvenements();
	return 0;
}

void gestionEvenement(EvenementGfx evenement) {
	static bool accelerate = false, pleinEcran = false;
	static individu tabIndividu[10];
	static nourriture tabNourriture[20];
	static int score = 0, evolution = 0;
	static char afficheScore[100], afficheEvolution[100];
	
	switch (evenement) {
		case Initialisation:
			generationNourriture(tabNourriture);
			generationIndividu(tabIndividu);
			demandeTemporisation(20);
			break;
		
		case Temporisation:
			// Déplacement des individus
			deplacement(tabIndividu);
			// Ramassage de la nourriture
			ramasse(tabIndividu, tabNourriture, &score);
			// Sélection automatique
			if (score >= 100) {
				score = 0;
				selection(tabIndividu);
				evolution += 1;
			}
			rafraichisFenetre();
			break;
			
		case Affichage:
			// Départ écran blanc
			effaceFenetre (255, 255, 255);
			// Affichage de la maison
			couleurCourante(215, 0, 0);
			cercle(largeurFenetre()/2, hauteurFenetre()/2+hauteurFenetre()/16, 75);
			// Affichage des individus
			afficheIndividu(tabIndividu);
			// Affichage de la nourriture
			afficheNourriture(tabNourriture);
			// Traçage d'un rectangle blanc
			couleurCourante(255, 255, 255);
			rectangle(0, 0, largeurFenetre(), hauteurFenetre()/16);
			// Traçage d'une ligne en bas de l'écran
			couleurCourante(0, 0, 0);
			epaisseurDeTrait(2);
			ligne(0, hauteurFenetre()/16, largeurFenetre(), hauteurFenetre()/16);
			// Affiche d'une chapine de caractère à l'écran
			couleurCourante(0, 0, 0);
			epaisseurDeTrait(2);
			sprintf(afficheScore, "Score: %d", score);
			afficheChaine(afficheScore, hauteurFenetre()/32, 7*largeurFenetre()/8, hauteurFenetre()/64);
			sprintf(afficheEvolution, "Evolution n.%d", evolution);
			afficheChaine(afficheEvolution, hauteurFenetre()/32, largeurFenetre()/128, hauteurFenetre()/64);
			break;
			
		case Clavier:
			switch (caractereClavier()) {
				case 'A':
				case 'a':
					accelerate = !accelerate;
					acceleration(tabIndividu, accelerate);
					break;

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
					// Génération de la nourriture
					generationNourriture(tabNourriture);
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
			// Génération de la nourriture
			generationNourriture(tabNourriture);
			break;
	}
}
