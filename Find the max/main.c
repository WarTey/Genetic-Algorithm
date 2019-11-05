#include <stdio.h>
#include <math.h>
#include <stdbool.h>
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

void drawFonction(float intervalMin, float intervalMax) {
	epaisseurDeTrait(2);
	couleurCourante(255, 0, 0);
	for (int i = 0; i < intervalMax; i++) {
		ligne(i*(largeurFenetre()/intervalMax), pow(i, 2), (i+1)*(largeurFenetre()/intervalMax), pow(i+1, 2));
        cercle((i+1)*(largeurFenetre()/intervalMax), pow(i+1, 2), 3);
    }
}

void drawSolution(population *individu, int taillePop, float intervalMax) {
	couleurCourante(0, 255, 0);
    for (int i = 0; i < taillePop; i++)
		cercle(individu[i].valeur*(largeurFenetre()/intervalMax), individu[i].fitness, 3);
}

int main(int argc, char **argv) {
	initialiseGfx(argc, argv);
	prepareFenetreGraphique("Find the max", LargeurFenetre, HauteurFenetre);
	lanceBoucleEvenements();
	return 0;
}

void gestionEvenement(EvenementGfx evenement) {
    static bool pleinEcran = false;
	static float intervalMax = 0, intervalMin = 0;
    static int chanceMutation = 0;
    static int generation = 0;
    static population individu[POPULATION_SIZE];
	
	switch (evenement) {
		case Initialisation:
            printf("Fonction : x^2\n");
            do {
                printf("Saisir la borne min d'étude: ");
                scanf("%f", &intervalMin);
            } while (intervalMin < 0);
            do {
                printf("Saisir la borne max d'étude: ");
                scanf("%f", &intervalMax);
            } while (intervalMax < intervalMin);
            do {
                printf("Saisir la chance d'appliquer une mutation sur un gène (0-100): ");
                scanf("%d", &chanceMutation);
            } while (chanceMutation < 0 || chanceMutation > 100);
            initPop(individu, POPULATION_SIZE, intervalMin, intervalMax);
            affichePop(individu, POPULATION_SIZE, generation);
            stats(individu, POPULATION_SIZE, intervalMin, intervalMax);
			demandeTemporisation(200);
			break;
		
		case Temporisation:
			if (generation < 500) {
                generation += 1;
                crossover(individu, POPULATION_SIZE, intervalMin, intervalMax);
                mutation(individu, POPULATION_SIZE, intervalMin, intervalMax, chanceMutation);
                affichePop(individu, POPULATION_SIZE, generation);
                stats(individu, POPULATION_SIZE, intervalMin, intervalMax);
            }
			rafraichisFenetre();
			break;
			
		case Affichage:
			// Départ écran blanc
			effaceFenetre (255, 255, 255);
			drawFonction(intervalMin, intervalMax);
            drawSolution(individu, POPULATION_SIZE, intervalMax);
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