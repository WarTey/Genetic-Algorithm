#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lib/GfxLib.h"
#include "lib/BmpLib.h"
#include "lib/ESLib.h"

#define LargeurFenetre 1200
#define HauteurFenetre 720

typedef struct individu {
	float xProb, yProb;
	float x, y;
	int panier;
	int vitesse;
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

void genereIndividu(individu tabIndividu[10]) {
	// Affiche les individus
	char scoreIndividu[100];
	for (int i = 0; i < 10; i++) {
		couleurCourante(0, 0, 215);
		cercle(tabIndividu[i].x, tabIndividu[i].y, 15);
		couleurCourante(255, 255, 255);
		epaisseurDeTrait(2);
		sprintf(scoreIndividu, "%d", tabIndividu[i].total + tabIndividu[i].panier);
		afficheChaine(scoreIndividu, 10, tabIndividu[i].x-5, tabIndividu[i].y-5);
	}
}

void genereNourriture(nourriture tabNourriture[20]) {
	// Affiche la nourriture
	couleurCourante(0, 125, 0);
	for (int i = 0; i < 20; i++)
		rectangle(tabNourriture[i].x-5, tabNourriture[i].y-5, tabNourriture[i].x+5, tabNourriture[i].y+5);
}

void deplacement(individu tabIndividu[10]) {
	// Déplacement aléatoire de l'individu en fonction de ses gene
	for (int i = 0; i < 10; i++) {
		tabIndividu[i].x = valeurIntervalleZeroUn() > tabIndividu[i].xProb ? tabIndividu[i].x - tabIndividu[i].vitesse : tabIndividu[i].x + tabIndividu[i].vitesse;
		tabIndividu[i].y = valeurIntervalleZeroUn() > tabIndividu[i].yProb ? tabIndividu[i].y - tabIndividu[i].vitesse : tabIndividu[i].y + tabIndividu[i].vitesse;
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
	for (int i = 0; i < 10; i++) {
		if (accelerate)
			tabIndividu[i].vitesse *= 2;
		else
			tabIndividu[i].vitesse /= 2;
	}
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
	static nourriture tabNourriture[20];
	static int score;
	static char afficheScore[100];
	static bool accelerate;
	
	switch (evenement) {
		case Initialisation:
			score = 0;
			accelerate = false;
			for (int i = 0; i < 20; i++) {
				tabNourriture[i].x = 0;
				tabNourriture[i].y = 0;
				while (tabNourriture[i].x <= 5 || tabNourriture[i].y <= hauteurFenetre()/16+5 || (tabNourriture[i].x >= largeurFenetre()/2-80 && tabNourriture[i].x <= largeurFenetre()/2+80 && tabNourriture[i].y >= hauteurFenetre()/2+hauteurFenetre()/16-80 && tabNourriture[i].y <= hauteurFenetre()/2+hauteurFenetre()/16+80)) {
					tabNourriture[i].x = valeurAleatoire()*largeurFenetre();
					tabNourriture[i].y = valeurAleatoire()*hauteurFenetre();
				}
				if (i < 10) {
					tabIndividu[i].panier = 0;
					tabIndividu[i].total = 0;
					tabIndividu[i].x = largeurFenetre()/2;
					tabIndividu[i].y = hauteurFenetre()/2+hauteurFenetre()/16;
					tabIndividu[i].xProb = valeurIntervalleZeroUn();
					tabIndividu[i].yProb = valeurIntervalleZeroUn();
					tabIndividu[i].vitesse = valeurIntervalleZeroUn()*4+1;
				}
			}
			demandeTemporisation(20);
			break;
		
		case Temporisation:
			//printf("Score: %d\n", score);
			// Déplacement des individus
			deplacement(tabIndividu);
			// Ramassage de la nourriture
			ramasse(tabIndividu, tabNourriture, &score);
			rafraichisFenetre();
			break;
			
		case Affichage:
			// Départ écran blanc
			effaceFenetre (255, 255, 255);
			// Affichage de la maison
			couleurCourante(215, 0, 0);
			cercle(largeurFenetre()/2, hauteurFenetre()/2+hauteurFenetre()/16, 75);
			// Génération des individus
			genereIndividu(tabIndividu);
			// Génération de la nourriture
			genereNourriture(tabNourriture);
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
			afficheChaine("Appuyez sur 'M' ou 'm' pour une mutation.", hauteurFenetre()/32, largeurFenetre()/128, hauteurFenetre()/64);
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
					for (int i = 0; i < 20; i++) {
						tabNourriture[i].x = 0;
						tabNourriture[i].y = 0;
						while (tabNourriture[i].x <= 5 || tabNourriture[i].y <= hauteurFenetre()/16+5 || (tabNourriture[i].x >= largeurFenetre()/2-80 && tabNourriture[i].x <= largeurFenetre()/2+80 && tabNourriture[i].y >= hauteurFenetre()/2+hauteurFenetre()/16-80 && tabNourriture[i].y <= hauteurFenetre()/2+hauteurFenetre()/16+80)) {
							tabNourriture[i].x = valeurAleatoire()*largeurFenetre();
							tabNourriture[i].y = valeurAleatoire()*hauteurFenetre();
						}
					}
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
			for (int i = 0; i < 20; i++) {
				tabNourriture[i].x = 0;
				tabNourriture[i].y = 0;
				while (tabNourriture[i].x <= 5 || tabNourriture[i].y <= hauteurFenetre()/16+5 || (tabNourriture[i].x >= largeurFenetre()/2-80 && tabNourriture[i].x <= largeurFenetre()/2+80 && tabNourriture[i].y >= hauteurFenetre()/2+hauteurFenetre()/16-80 && tabNourriture[i].y <= hauteurFenetre()/2+hauteurFenetre()/16+80)) {
					tabNourriture[i].x = valeurAleatoire()*largeurFenetre();
					tabNourriture[i].y = valeurAleatoire()*hauteurFenetre();
				}
			}
			break;
	}
}
