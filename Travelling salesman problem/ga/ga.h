typedef struct individu {
	int chemin[10];
	float fitness;
} individu;

typedef struct ville {
	float x, y;
} ville;

void fitness(individu tabIndividu[10], ville tabVille[10]);
void afficheGeneration(individu tabIndividu[10], int generation);
void crossover(individu tabIndividu[10]);
void mutation(individu tabIndividu[10]);