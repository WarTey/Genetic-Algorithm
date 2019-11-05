#define POPULATION_SIZE 10

typedef struct population {
    float valeur;
    float fitness;
} population;

void stats(population *individu, int taillePop, float intervalMin, float intervalMax);
void initPop(population *individu, int taillePop, float intervalMin, float intervalMax);
void affichePop(population *individu, int taillePop, int generation);
void crossover(population *individu, int taillePop, float intervalMin, float intervalMax);
void mutation(population *individu, int taillePop, float intervalMin, float intervalMax, float chanceMutation);