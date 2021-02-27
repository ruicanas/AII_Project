#define MAX_OBJ 300

typedef struct informacao
{
	// Tamanho da popula��o
	int     popsize;
	// Probabilidade de muta��o
	double   pm;
	// Probabilidade de recombina��o
	double   pr;
	// Tamanho do torneio para sele��o do pai da pr�xima gera��o
	int     tsize;
	// Constante para avalia��o com penaliza��o
	float   ro;
	// N�mero de objetos que se podem colocar na mochila
	int     numGenes;
	// N�mero de gera��es
	int     numGenerations;
	// N�mero de arestas
	int		ar;
}info;

// Individuo (solu��o)
typedef struct individual
{
	// Solu��o (objetos que est�o dentro da mochila)
	int     solInd[MAX_OBJ];
	// Valor da qualidade da solu��o
	int   fitness;
	// 1 se for uma solu��o v�lida e 0 se n�o for
	int     valido;
}chrom, *pchrom;


int* init_dados(char* nome, int* vertices, int* arestas);
void gera_sol_inicial(int* sol, int v);
info preenchePara(int vertices, int arestas);
chrom get_best(pchrom pop, info d, chrom best);
void write_best(chrom x, info d);
pchrom init_pop(info d);
void escreve_sol(int* sol, int vert);
void substitui(int a[], int b[], int n);
void init_rand();
chrom escolha_inicial(pchrom pop, info d);
int random_l_h(int min, int max);
float rand_01();
