#define NB_NOURRITURES 20
#define NB_INDIVIDUS 10

// Contient les informations des individus
typedef struct individu {
	float xVitesse, yVitesse;
	float xProb, yProb;
	float x, y;
	int panier;
	int total;
	int last;
} individu;

void transformation(individu tabIndividu[NB_INDIVIDUS]);