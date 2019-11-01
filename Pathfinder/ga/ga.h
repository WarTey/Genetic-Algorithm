#define NB_INDIVIDUS 10
#define NB_CHEMINS 10

typedef struct individu {
	float angle[NB_CHEMINS-1];
	float x[NB_CHEMINS], y[NB_CHEMINS];
	float fitness;
} individu;

typedef struct zone {
	float x, y;
	int taille;
} zone;

void fitness(individu tabIndividu[NB_INDIVIDUS], zone arrivee);
void afficheGeneration(individu tabIndividu[NB_INDIVIDUS], int generation);
void crossover(individu tabIndividu[NB_INDIVIDUS]);
void computeCoord(individu *tabIndividu);
void mutation(individu tabIndividu[NB_INDIVIDUS]);