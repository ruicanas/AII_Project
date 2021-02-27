#include "util.h"
#include "funcaoce.h"
#include <math.h>
#define GERAVIZ 0.8

//Recebe: solução (sol[]), os parametros (d), a matriz (mat) e o número de vértices
//Devolve a validade da população e o respetivo fitness indiretamente (através de ponteiros)
int eval_individual(int sol[], info d, int *mat, int* v)
{
	int flag = 0;
	int best = 0;
	int conta = 0;
	int total = 0;
	int valor1, valor2;
	int i, j = 0;

	// Percorre todos os objectos para ver se a solução tem algum número repetido
	// Uma vez que vai ser este o fator de de invalidez.
	// (Código utilizado apenas para testes de forma a confirmar aquilo que seria de esperar
	// que é uma população apenas com soluções válidas)
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
		// Solução inválida se tiver uma solução com números repetidos!
		*v = 0;
		return 0;
	}
	else
	{
		// Solução válida se tiver uma solução com números todos diferentes!
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

//Recebe: População, os parametros e a matriz lida
//Vai correr cada população individualmente e avalia-la com o eval_individual
void evaluate(pchrom pop, info d, int *mat)
{
	int i;

	for (i = 0; i < d.popsize; i++) {
		pop[i].fitness = eval_individual(pop[i].solInd, d, mat, &pop[i].valido);
	}
}

//Gera Vizinho no método hibrido, que dada uma probabilidade gera vizinhança
//Recebe: a solução(sol), uma solução vizinha que vai ser posteriormente preenchida(solViz), e a nº da população(nGenes)
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

//Trepa Colinas no método híbrido
//Recebe: a população(pop), os parametros(d) e a matriz(mat);
void trepa_colinasH(pchrom pop, info d, int *mat)
{
	int     i, j;
	chrom   vizinho;

	for (i = 0; i < d.popsize; i++) // CICLO QUE VAI PERCORRER CADA UMA DAS SOLUÇÕES
	{
		for (j = 0; j < d.numGenerations; j++)
		{
			gera_vizinhoH(pop[i].solInd, vizinho.solInd, d.numGenes);
			vizinho.fitness = eval_individual(vizinho.solInd, d, mat, &vizinho.valido);
			if (vizinho.fitness >= pop[i].fitness) // Comparar o vizinho com a qualidade da que está a ser utilizada atualmente.
				pop[i] = vizinho;
		}
	}
}