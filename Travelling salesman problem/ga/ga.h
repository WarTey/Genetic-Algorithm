#define NB_INDIVIDUS 10
#define NB_VILLES 10

typedef struct individu {
	int chemin[NB_VILLES];
	float fitness;
} individu;

typedef struct ville {
	float x, y;
} ville;

void fitness(individu tabIndividu[NB_INDIVIDUS], ville tabVille[NB_VILLES]);
void afficheGeneration(individu tabIndividu[NB_INDIVIDUS], int generation);
void crossover(individu tabIndividu[NB_INDIVIDUS]);
void mutation(individu tabIndividu[NB_INDIVIDUS]);