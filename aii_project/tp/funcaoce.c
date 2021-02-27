#include "util.h"
#include "funcaoce.h"
#include <math.h>
#define GERAVIZ 0.8

//Recebe: solu��o (sol[]), os parametros (d), a matriz (mat) e o n�mero de v�rtices
//Devolve a validade da popula��o e o respetivo fitness indiretamente (atrav�s de ponteiros)
int eval_individual(int sol[], info d, int *mat, int* v)
{
	int flag = 0;
	int best = 0;
	int conta = 0;
	int total = 0;
	int valor1, valor2;
	int i, j = 0;

	// Percorre todos os objectos para ver se a solu��o tem algum n�mero repetido
	// Uma vez que vai ser este o fator de de invalidez.
	// (C�digo utilizado apenas para testes de forma a confirmar aquilo que seria de esperar
	// que � uma popula��o apenas com solu��es v�lidas)
	for (i = 0; i < d.numGenes; i++) {
		j = j++;
		for (j = i+1 ; j < d.numGenes; j++) {
			if (sol[i] == sol[j]) {
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			break;
	}
	
	if (flag == 1)
	{
		// Solu��o inv�lida se tiver uma solu��o com n�meros repetidos!
		*v = 0;
		return 0;
	}
	else
	{
		// Solu��o v�lida se tiver uma solu��o com n�meros todos diferentes!
		for (j = 0; j < 2; j++) {
			if (j == 1) {
				valor1 = *(mat + i * 2 + j);
			}
			else {
				valor2 = *(mat + i * 2 + j);
			}
		}
		total = abs(sol[valor2 - 1] - sol[valor1 - 1]);
		if (total > best)
			best = total;
		
		*v = 1;
		return best;
	}
}

//Recebe: Popula��o, os parametros e a matriz lida
//Vai correr cada popula��o individualmente e avalia-la com o eval_individual
void evaluate(pchrom pop, info d, int *mat)
{
	int i;

	for (i = 0; i < d.popsize; i++) {
		pop[i].fitness = eval_individual(pop[i].solInd, d, mat, &pop[i].valido);
	}
}

//Gera Vizinho no m�todo hibrido, que dada uma probabilidade gera vizinhan�a
//Recebe: a solu��o(sol), uma solu��o vizinha que vai ser posteriormente preenchida(solViz), e a n� da popula��o(nGenes)
void gera_vizinhoH(int sol[], int solViz[], int nGenes)
{
	if (rand_01() < GERAVIZ) {
		int i, p1, p2, temp;
		for (i = 0; i < nGenes; i++)
			solViz[i] = sol[i];

		p1 = random_l_h(0, nGenes - 1);
		do {
			p2 = random_l_h(0, nGenes - 1);
		} while (p2 == p1);

		temp = solViz[p1];
		solViz[p1] = solViz[p2];
		solViz[p2] = temp;
	}
}

//Trepa Colinas no m�todo h�brido
//Recebe: a popula��o(pop), os parametros(d) e a matriz(mat);
void trepa_colinasH(pchrom pop, info d, int *mat)
{
	int     i, j;
	chrom   vizinho;

	for (i = 0; i < d.popsize; i++) // CICLO QUE VAI PERCORRER CADA UMA DAS SOLU��ES
	{
		for (j = 0; j < d.numGenerations; j++)
		{
			gera_vizinhoH(pop[i].solInd, vizinho.solInd, d.numGenes);
			vizinho.fitness = eval_individual(vizinho.solInd, d, mat, &vizinho.valido);
			if (vizinho.fitness >= pop[i].fitness) // Comparar o vizinho com a qualidade da que est� a ser utilizada atualmente.
				pop[i] = vizinho;
		}
	}
}