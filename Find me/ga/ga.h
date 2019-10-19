// Taille du dictionnaire et du mot à saisir
#define T_DIC 27
#define T_MOT 10

typedef char chaine[T_DIC];
typedef char mot[T_MOT+1];

typedef struct population {
    mot gene;
    int fitness;
} population;

void fitness(population *initPop, mot answer, bool *found, int taillePop);
void afficheGeneration(population *initPop, int generation, int taillePop);
void frise(population *initPop, mot gene, int total, int taillePop);
void paire(population *initPop, population pairePop[][2], int taillePop);
void create(mot gene, population pairePop[][2], int indice, int taillePop);
void crossover(population *initPop, int taillePop);
void mutation(population *initPop, chaine dic, int taillePop);