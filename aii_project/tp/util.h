#define MAX_OBJ 300

typedef struct informacao
{
	// Tamanho da população
	int     popsize;
	// Probabilidade de mutação
	double   pm;
	// Probabilidade de recombinação
	double   pr;
	// Tamanho do torneio para seleção do pai da próxima geração
	int     tsize;
	// Constante para avaliação com penalização
	float   ro;
	// Número de objetos que se podem colocar na mochila
	int     numGenes;
	// Número de gerações
	int     numGenerations;
	// Número de arestas
	int		ar;
}info;

// Individuo (solução)
typedef struct individual
{
	// Solução (objetos que estão dentro da mochila)
	int     solInd[MAX_OBJ];
	// Valor da qualidade da solução
	int   fitness;
	// 1 se for uma solução válida e 0 se não for
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
